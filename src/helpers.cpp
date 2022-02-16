#include "helpers.h"

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
bool getContours(Mat imgDil, Mat img, int &x, int &y) {

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f> centers( contours.size() );
    vector<float> radius( contours.size() );
    long unsigned int i; 
    for (i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        //cout << area << endl;
        string objectType;

        if (area > 1000) 
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            //cout << conPoly[i].size() << endl;
            boundRect[i] = boundingRect(conPoly[i]);
            minEnclosingCircle( conPoly[i], centers[i], radius[i] );
        
            int objCor = (int)conPoly[i].size();

            objectType = "Tennis Ball"; 

            //drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            x = (int)centers[i].x;
            y = (int)centers[i].y;
            
            circle(img, centers[i], 3, Scalar(0,100,100), 3, LINE_AA);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
            putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN,1, Scalar(0, 69, 255), 2);
            return true;            
        }
    }
    return false;
}


/**
 * @brief 
 *
 * @param src
 * @param ballX
 * @param ballY
 */
void quadrant(Mat src, int ballX, int ballY)
{
    Mat imgDil, imgErode, gradX, imgHSV, absGradX, thr;
   
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(0, 0, 200), Scalar(180, 50, 255), thr);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(21, 21));
    erode(thr, imgErode, kernel);

    // Preprocessing
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(imgErode, linesP, 1, CV_PI/180, 80, 350, 30 ); // runs the actual detection
    
    // Draw the lines
    //cout << linesP.size() << endl;
    float mNeg = 0;
    float mPos = -1;
    float bNeg = 0;
    float bPos = 0;
    int length = 50;
    int x1, y1, x2, y2;
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        x1 = l[0];
        y1 = l[1];
        x2 = l[2];
        y2 = l[3];


        if (abs(x2 - x1) > length) {
            float m = (float)(y2 - y1) / (float)(x2 - x1);
            if (m >= 0) { // handle zero later
                mPos = m;
                bPos = y1 - m*x1;
        
            }
        
            else {
                mNeg = m;
                bNeg = y1 - m*x1;
        
            }
            line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, LINE_AA);
        }

        if (mPos >= 0 && mNeg < 0) {
            //cout << mPos << "x + " << bPos << "\n" << endl;
            //cout << mNeg << "x + " << bNeg << "\n" << endl; 
            break;
        }
    }
    int negY = mNeg * ballX + bNeg;
    int posY = mPos * ballX + bPos;
    string quad = "Line";
    if (ballY <= negY) {
        if (ballY > posY) {
            quad = "1";
        }
        else {
            quad = "2";
        }
    }
    else {
        if (ballY > posY) {
            quad = "4";
        }
        else {
            quad =  "3";
        }
    }
    putText(src, quad, { 100, 100 }, FONT_HERSHEY_PLAIN, 10, Scalar(0, 69, 255), 10);

}



#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest/doctest.h"
TEST_CASE("we can have tests written here, to test impl. details")
{
    CHECK(true);
}
#endif
