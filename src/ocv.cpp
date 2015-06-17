#include "ocv.hpp"

using namespace cv;

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

void initFrame()
{
	namedWindow( "Display window", CV_GUI_EXPANDED );
}



void showFrame(Mat & cgBuffer)
{
	resize(cgBuffer, cgBuffer,  cv::Size(), 0.8, 0.8, INTER_CUBIC);
	imshow("Display window", cgBuffer);
}
