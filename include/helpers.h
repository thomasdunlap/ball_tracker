#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

/**
 * @brief 
 *
 * @param imgDil
 * @param img
 * @param x
 * @param y
 *
 * @return 
 */
bool getContours(Mat imgDil, Mat img, int &x, int &y);

/**
 * @brief 
 *
 * @param src
 * @param ballX
 * @param ballY
 */
void quadrant(Mat src, int ballX, int ballY);

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("we can have tests in headers if we want")
{
    CHECK(true);
}
#endif
