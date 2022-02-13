// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <stdlib.h>

#include "exampleConfig.h"
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


int main(int , char** argv)
{
	// initialize a video capture object
	VideoCapture vid_capture(argv[1]);
	int fps = 30;
	int frame_count = 120;
	// Print error message if the stream is invalid
	if (!vid_capture.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
	}

	else
	{
		// Obtain fps and frame count by get() method and print
		// You can replace 5 with CAP_PROP_FPS as well, they are enumerations
		fps = vid_capture.get(5);
		cout << "Frames per second :" << fps;

		// Obtain frame_count using opencv built in frame count reading method
		// You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
		frame_count = vid_capture.get(7);
		cout << "  Frame count :" << frame_count;
	}

	// Obtain frame size information using get() method
	int frame_width = static_cast<int>(vid_capture.get(3));
	int frame_height = static_cast<int>(vid_capture.get(4));
	Size frame_size(frame_width, frame_height);
	//int fps = 120;

	VideoWriter output("./output.avi",
			   VideoWriter::fourcc('M', 'J', 'P', 'G'),
			   fps, frame_size);

	while (vid_capture.isOpened())
    {
		// Initialize frame matrix
		Mat frame, grayImg, binaryImg;

		// Initialize a boolean to check if frames are there or not
		bool isSuccess = vid_capture.read(frame);

		// If frames are not there, close it
		if (isSuccess == false)
		{
			cout << "Stream disconnected" << endl;
			break;
		}


		// If frames are present
		if(isSuccess == true)
		{
			
			Mat imgHSV, thr;
			int x = -1;
			int y = -1;
			cvtColor(frame, imgHSV, COLOR_BGR2HSV);
			
			inRange(imgHSV, Scalar(0, 69, 50), Scalar(108, 255, 255), thr);
			medianBlur(thr, thr, 5);

			if (getContours(thr, frame, x, y)) {
				quadrant(frame, x, y);
			}
			imshow("Image", frame);
			output.write(frame);

			// the loop if key q is pressed
			int key = waitKey(60);
			if (key == 'q')
			{
				cout << "q key pressed. Stopping the video" << endl;
				break;
			}
		}
	}

	// Release the video capture object
	vid_capture.release();
	destroyAllWindows();

	return 0;
}

