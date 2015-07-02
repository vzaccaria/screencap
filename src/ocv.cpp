#include "ocv.hpp"
#include "ocvs.hpp"
#include <algorithm>
#include <utility>
#include <mutex>

using namespace cv;


void drawBlackRect(Mat & canvas, cv::Rect r) {
	cv::rectangle(canvas, r, black, CV_FILLED);
}

void convertImage(CGImageRef imageRef, Mat & cgBuffer)
{
	auto cols       = CGImageGetWidth(imageRef);
	auto rows       = CGImageGetHeight(imageRef);
	auto colorSpace = CGImageGetColorSpace(imageRef);

	cgBuffer.create(rows, cols, CV_8UC4);

	CGContextRef contextRef = CGBitmapContextCreate(
		cgBuffer.data,                            // Pointer to backing data
		cols,                                     // Width of bitmap
		rows,                                     // Height of bitmap
		8,                                        // Bits per component
		cgBuffer.step[0],                         // Bytes per row
		colorSpace,                               // Colorspace
		kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault
		);                                        // Bitmap info flags

	CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), imageRef);
	CGContextRelease(contextRef);
	CGImageRelease(imageRef);
}

float gWidth;
float gHeight;

void initFrame(int width, int height)
{
	namedWindow( "Display window", 0);
	resizeWindow( "Display window", width, height);
	
	gWidth = width;
	gHeight = height;
}


void overlayTime(Mat & dBuffer) {
	// splitV(dBuffer, 0.30, lambda(d) {
	// 		overlayOn(d, 0.2, lambda(canvas) {
	// 				drawBlackRect(canvas, pad(canvas, 0.1).first);
	// 			});
			cv::putText(dBuffer, "pippo", cv::Point(50,50), FONT_HERSHEY_DUPLEX, 1.0, white);
		// });
}

// void overlayText(Mat & dBuffer, const char *text) {
// 	overlayOn(dBuffer, 0.2, lambda(canvas) {
// 			cv::rectangle(canvas, cv::Rect(20, 20, 200, 50), black, CV_FILLED);
// 		});
// 	overlayOn(dBuffer, 0.9, lambda(canvas) {
// 			cv::putText(canvas, text, cv::Point(50,50), FONT_HERSHEY_DUPLEX, 1.0, white);
// 		});
// }

std::mutex buffAccess;

void showFrame(Mat &fromBuffer)
{
	Mat destBuffer(gWidth, gHeight, CV_8UC4);

	auto displayTime = lambda(d1Buffer) {
			overlayTime(d1Buffer);
	};

	auto resizeVideo = lambda(d2Buffer) {
		resizeKeepAspectRatio(fromBuffer, d2Buffer);
		//resizeKeepAspectRatio(fromBuffer, destBuffer);
	};
	
	splitV(destBuffer, 0.2, displayTime, resizeVideo);
    std::lock_guard<std::mutex> guard(buffAccess);
	imshow("Display window", destBuffer);
}
