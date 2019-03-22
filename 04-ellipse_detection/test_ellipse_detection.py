#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 14 16:12:25 2017

@author: jason
"""

import sys
sys.path.append('..')

import cv2

import ellipse_candidate as ec
import ellipse_candidate_maker as ecm
import ellipse_center_estimator as ece
import ellipse_detector as ed
import ellipse_estimator as ee
import ellipse_merger as em
import __init__
import segment
import segment_detector as sd
import segment_pair as sp
import test_ellipse_detection as ted


def main():
    image = cv2.imread('01.jpg', 1)
    # image = cv2.imread('../images/sP1010080.jpg', 1)
    # image = cv2.imread('../images/20091031193703238.jpg', 1)
    # image = cv2.imread('../images/KM-612.jpg', 1)
    # image = cv2.imread('../images/49days-301.jpg', 1)
    # image = cv2.imread('../images/ellipse.png', 1)
    # image = cv2.imread('../images/83I83Z838D82P89F196DA81B.JPG', 1)
    # image = cv2.imread('../images/20120827-003.jpg', 1)
    # image = cv2.imread('../images/ellipse2.png', 1)
    # mage = cv2.imread('../images/rot-ellipse.png', 1)
    # image = cv2.imread('../images/rot-ellipse-2.png', 1)

    image_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    seg_det = sd.SegmentDetector()
    segments = seg_det.detect(image_gray)

    ellipse_cand_maker = ecm.EllipseCandidateMaker()
    ellipse_cands = ellipse_cand_maker.make(segments)
    #debug_image = image.copy()
    #ellipse_cands = ellipse_cand_maker.make(debug_image)
    print 'ellipse_cands =', ellipse_cands

    ellipse_estimator = ee.EllipseEstimator()
    ellipses = ellipse_estimator.estimate(ellipse_cands)

    print 'before merge = ', len(ellipses)
    ellipse_merger = em.EllipseMerger(image_gray.shape[1], image_gray.shape[0])
    ellipses = ellipse_merger.merge(ellipses)
    print 'after merge = ', len(ellipses)

    for ellipse in ellipses:
        print ellipse
        image_ellipse = image.copy()
        ellipse.draw(image_ellipse)
        cv2.imshow('ellipse', image_ellipse)
        cv2.waitKey(0)


if __name__ == '__main__':
    main()

#main()
