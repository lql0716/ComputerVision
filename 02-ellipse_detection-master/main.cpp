#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

Mat src;

struct ellipse_data
{

    ellipse_data(): x0(-1), y1(-1) {};
    ellipse_data(int _x0, int _y0, double _a, double _b, double th);
    ellipse_data(int _x0, int _y0, double _a, double _b, double th, int _x1, int _y1, int _x2, int _y2);

    double a, b;
    int x0, y0;
    int x1, y1, x2, y2;

    double orient;
};

inline double distance_square(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

inline pair<int,int> compute_center(int x1, int y1, int x2, int y2)
{
    return make_pair(cvRound((x1 + x2)/2), cvRound((y1 + y2)/2));
}

inline double major_axis_length_sq(int x1, int y1, int x2, int y2)
{
    return distance_square(x1, y1, x2, y2) / 4;
}

inline double orientation(double x1, double y1, double x2, double y2) {
    if (x1 == x2)
        return CV_PI / 2;
    return atan((y2 - y1) / (x2 - x1));
}

vector<vector<vector<short> > > store_hiearchical_pyramid(Mat& edgesM, int min_size = 32) //create hierarchical pyramid of minimized images
{
    int log2h = (int)floor(log2(edgesM.rows));
    int log2w = (int)floor(log2(edgesM.cols));
    int log2min = min(log2h, log2w);
    unsigned int max_size = pow(2, log2min);
    Mat resized = Mat(max_size,max_size,edgesM.type());
    resize(edgesM, resized, resized.size()); //resize to square

    unsigned int steps = (unsigned int)log2min - (unsigned int)floor(log2(min_size)) + 1;
    vector<vector<vector<short> > > pyramid = vector<vector<vector<short> > >(steps);
    pyramid[0] = vector<vector<short> >(max_size,vector<short>(max_size,0)); //creating the first image (original size)
    for (int i = 0; i < max_size; ++i) {
        for (int j = 0; j < max_size; ++j) {
            uchar p = resized.at<uchar>(i,j);
            if (p > 0)
                pyramid[0][i][j] = 1;
        }
    }

    unsigned int sz = max_size; //create next levels by summing pixels of previous ones.
    for (int i = 1; i < steps; i++)
    {
        sz /= 2;
        pyramid[i] = vector<vector<short> >(sz,vector<short>(sz,0));
        for (int j = 0; j < sz; j++)
        {
            for (int k = 0; k < sz; ++k) {
                pyramid[i][j][k] = pyramid[i-1][j*2][k*2] + pyramid[i-1][j*2+1][k*2] + pyramid[i-1][j*2][k*2+1] + pyramid[i-1][j*2+1][k*2+1];
            }
        }
    }
    return pyramid;
}

bool compr2(pair<ellipse_data, int> a, pair<ellipse_data, int> b) {
    return a.second < b.second;
}

inline bool can_belong_to_ellipse(int x, int y, ellipse_data e, double eps = 0.1) //for cleaning the image
{
    double _x = cos(e.orient) * (x - e.x0) + sin(e.orient) * (y - e.y0);
    double _y = sin(e.orient) * (x - e.x0) - cos(e.orient) * (y - e.y0);
    double eqt = (_x * _x)/(e.a*e.a) + (_y * _y)/(e.b*e.b);
    return abs(eqt - 1) < eps;
}

void voting(vector<pair<ellipse_data, int> > &ellipses, const vector<vector<short> > &data, pair<int, int> xbounds,
            pair<int, int> ybounds, int min_dist, int min_vote, pair<int, int> p0, pair<int, int> p1) {
    int maxlen = cvRound(sqrt(data.size() * data.size() + data[0].size() * data[0].size()));
    int acc[maxlen];
    for (int i = 0; i < maxlen; ++i) { //clear the accumulator array
        acc[i] = 0;
    }
    int x1 = p0.first;
    int y1 = p0.second;
    int x2 = p1.first;
    int y2 = p1.second;

    pair<int, int> center = compute_center(x1, y1, x2, y2); //get the center point of the ellipse

    int x0 = center.first;
    int y0 = center.second;

    double a_sq = major_axis_length_sq(x1, y1, x2, y2); //major axis (half)
    double a = sqrt(a_sq);

    if (a < min_dist)
        return;

    double orient = orientation(x1, y1, x2, y2); //ellipse can be rotated


    for (int y = ybounds.first; y < ybounds.second; y++) {
        for (int x = xbounds.first; x < xbounds.second; x++) //point of the ellipse (x,y)
        {
            if (((x == x1) && (y == y1)) ||
                ((x == x2) && (y == y2)) ||
                (data[y][x] == 0)) //the same as one of the side points or not a boundary point
                continue;

            double d_sq = distance_square(x, y, x0, y0);
            double d = sqrt(d_sq); //distance from the center

            if ((d < min_dist) || (d > a)) //if it's too small - it is probably a line
                //if bigger than the major axis - not an ellipse point
                continue;


            double f_sq = min(distance_square(x, y, x1, y1),
                              distance_square(x, y, x2, y2)); //distance to the nearest side point
            double costau = (a_sq + d_sq - f_sq) / (2 * a * d);
            double costau_sq = costau * costau;
            if (costau_sq > 1) {
                costau_sq = 1;
            }
            if (costau_sq < 0) {
                costau_sq = 0;
            }
            double sintau_sq = 1 - costau_sq;
            double b_sq = (a_sq * d_sq * sintau_sq) / (a_sq - d_sq * costau_sq);
            int b = cvRound(sqrt(b_sq));
            if (b > min_dist) { //voting
                acc[b] += data[y][x];
            }
        }
    }
    //here we get the most-likely minor axis length of the assumed ellipse and how many points would such an ellipse contain
    int maxlength = 0;
    int maxvote = 0;
    for (int k = 0; k < maxlen; ++k) {
        if (acc[k] >= maxvote) {
            maxvote = acc[k];
            maxlength = k;
        }
        //acc[k] = 0;
    }

    if (maxvote > min_vote) {
        ellipses.push_back(make_pair(ellipse_data(x0, y0, a, maxlength, orient, x1, y1, x2, y2), maxvote));
    }
}

//First search (should be made on the minimized image)
vector<pair<ellipse_data, int> > hough_transform(vector<vector<short> > &data, int min_vote = 1, int min_dist = 0)
{
    pair<int, int> xbounds = make_pair(0, (int) data.size());
    pair<int, int> ybounds = make_pair(0, (int) data[0].size());
    vector<pair<ellipse_data, int> > res = vector<pair<ellipse_data, int> >();

    for (int y1 = ybounds.first; y1 < ybounds.second; y1++)
    {
        for (int x1 = xbounds.first; x1 < xbounds.second; x1++) //point (x1, y1) - side point of major axis
        {
            if (data[y1][x1] == 0) //not a boundary point
                continue;

            for (int y2 = y1; y2 < ybounds.second; y2++)
            {
                for (int x2 = xbounds.first; x2 < xbounds.second; x2++) //point (x2, y2) - side point of major axis
                {
                    if (((y2 == y1) && (x2 <= x1)) ||
                        (data[y2][x2] == 0)) //already been selected OR not a boundary point
                        continue;

                    voting(res, data, xbounds, ybounds, min_dist, min_vote, make_pair(x1, y1), make_pair(x2, y2));
                }
            }
        }
    }
    return res;
}

ellipse_data relocate_ellipse(vector<vector<short> > &data, ellipse_data prev, int min_vote = 1, int min_dist = 0)
{
    //The area where we search for ellipse points (basically, all the image)
    pair<int, int> xbounds = make_pair(0, (int) data.size());
    pair<int, int> ybounds = make_pair(0, (int) data[0].size());
    vector<pair<ellipse_data, int> > res;

    int maxlen = cvRound(sqrt(data.size() * data.size() + data[0].size() * data[0].size()));

    int acc[maxlen];
    for (int i = 0; i < maxlen; ++i) { //clear the accumulator array
        acc[i] = 0;
    }


    //This time, we search for side points only as far as +- pixel from previously found ones
    //Okay, +-2 pixels for a little bit higher accuracy (I believe).
    int sidex1_start = max(xbounds.first, 2 * prev.x1 - 2);
    int sidex1_fin = min(xbounds.second, 2 * prev.x1 + 2);
    int sidex2_start = max(xbounds.first, 2 * prev.x2 - 2);
    int sidex2_fin = min(xbounds.second, 2 * prev.x2 + 2);
    int sidey1_start = max(ybounds.first, 2 * prev.y1 - 2);
    int sidey1_fin = min(ybounds.second, 2 * prev.y1 + 2);
    int sidey2_start = max(ybounds.first, 2 * prev.y2 - 2);
    int sidey2_fin = min(ybounds.second, 2 * prev.y2 + 2);

    for (int y1 = sidey1_start; y1 <= sidey1_fin; y1++) {
        for (int x1 = sidex1_start; x1 <= sidex1_fin; x1++) //point (x1, y1) - side point of major axis
        {
            if (data[y1][x1] == 0)
                continue;

            for (int y2 = sidey2_start; y2 <= sidey2_fin; y2++) {
                for (int x2 = sidex2_start; x2 <= sidex2_fin; x2++) //point (x2, y2) - side point of major axis
                {
                    if (((y2 == y1) && x2 <= x1) || (data[y2][x2] == 0))
                        continue;

                    voting(res, data, xbounds, ybounds, min_dist, min_vote, make_pair(x1, y1), make_pair(x2, y2));
                }
            }
        }
    }
    pair<ellipse_data, int> e = *max_element(res.begin(), res.end(), compr2);
    return e.first;
}

void clear_picture(Mat& edges, ellipse_data elp)
{
    for (int x = 0; x < edges.cols; x++)
        for (int y = 0; y < edges.rows; y++)
        {
            if (can_belong_to_ellipse(x, y, elp, 0.2))
            {
                edges.at<uchar>(y,x) = 0;
            }
        }
}


ellipse_data ellipse_detection(Mat edges, int minimized_size = 64, int min_vote = 4, int min_dist = 5)
{
    vector<vector<vector<short> > > pyramid = store_hiearchical_pyramid(edges, minimized_size);

    int n = (int)pyramid.size();
    vector<pair<ellipse_data, int> > ellipses = hough_transform(pyramid[n - 1], min_vote, min_dist);

    if (ellipses.size() < 1) {
        cout << "Didn't find anything" << endl;
        return ellipse_data(-1, -1, -1, -1, 0);
    }
    cout << "Step 1 of " << n << endl;
    ellipse_data found = (*max_element(ellipses.begin(), ellipses.end(), compr2)).first;
    for (int i = n-2; i >= 0; i--)
    {
        cout << "Step " << n - i << " of " << n << endl;
        found = relocate_ellipse(pyramid[i], found);
    }
    return found;
}

vector<ellipse_data> detect_ellipses(Mat src, int minimized_size = 64, unsigned int number_of_ellipses = 1)
{
    vector<ellipse_data> ellipses = vector<ellipse_data>(number_of_ellipses);
    Mat edges;
    Canny(src, edges, 50, 200, 3);
    //Probably some other transformation should be performed here, such as blurring
    for (int i = 0; i < number_of_ellipses; i++)
    {
        ellipse_data elp = ellipse_detection(edges, minimized_size, 5, 15);
        ellipses[i] = elp;
        cout << "Found: (x_0 = " << elp.x0 << ", y_0 = " << elp.y0 << ", a = " << elp.a << ", b = " << elp.b <<
        ", A = " << elp.orient << ") " << endl;
        clear_picture(edges, elp);
    }
    return ellipses;

}

unsigned int nmb_of_ellipses = 1;
unsigned int minimised_size = 128;

String inp_img;
String out_img = "ellipses.jpg";

int main( int argc, char** argv ) {
    if (argc < 2) {
        cout <<
        "Usage: <input file name> [<output file name> (default: \"ellipses.jpg\") [<maximum number of ellipces to find (default: 1)>]]" <<
        endl;
        return 0;
    }
    inp_img = argv[1];

    if (argc > 2)
        out_img = argv[2];
    src = imread(inp_img, 1);
    if(! src.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    Mat ellipses_draw = src.clone();
    vector<ellipse_data> ellipses = detect_ellipses(src, minimised_size, nmb_of_ellipses);
    for (int i = 0; i < ellipses.size(); i++)
    {
        ellipse_data elp = ellipses[i];
        if ((elp.x0 >= 0) && (elp.y0 >=0))
        {
            ellipse(ellipses_draw, Point_<int>(elp.x0, elp.y0), Size_<double>(elp.a, elp.b), elp.orient * 180 / CV_PI,
                    0, 360, Scalar(0, 0, 255), 4, 8);
        }
    }
    imwrite(out_img, ellipses_draw);



    return 0;
}



ellipse_data::ellipse_data(int _x0, int _y0, double _a, double _b, double th): x0(_x0), y0(_y0), a(_a), b(_b), orient(th){
}

ellipse_data::ellipse_data(int _x0, int _y0, double _a, double _b, double th, int _x1, int _y1, int _x2, int _y2): x0(_x0), y0(_y0), a(_a), b(_b), orient(th), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
}
