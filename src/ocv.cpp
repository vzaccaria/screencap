#include "ocv.hpp"
#include "ocvs.hpp"
#include <algorithm>
#include <utility>
#include <mutex>
#include "time.hpp"

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


std::mutex buffAccess;

void showFrame(Mat &fromBuffer)
{
	Mat destBuffer(gWidth, gHeight, CV_8UC4);
   
    auto displayTime = lambda(canvas) {
		overlayOn(canvas, 0.2, lambda(c2) {
				auto rect = pad(canvas, 0.05).first;
				drawBlackRect(c2, rect);
			});
		padCanvas(canvas, 0.1, lambda(inner) {
				centerText(inner, curTime());
			});
    };
    
    resizeKeepAspectRatio(fromBuffer, destBuffer);
    splitV(destBuffer, 0.1, displayTime, lambda(foo) { });

    std::lock_guard<std::mutex> guard(buffAccess);
    imshow("Display window", destBuffer);
}
