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

float gWidth;
float gHeight;

void initFrame(int width, int height)
{
	namedWindow( "Display window", 0);
	resizeWindow( "Display window", width, height);
	
	gWidth = width;
	gHeight = height;
}

typedef struct sizeAndBorder {
	int hSize;
	int vSize;
	int hBorder;
	int vBorder;
} sizeAndBorder;

void resizeKeepAspectRatio(float wWidth, float wHeight, Mat & in, Mat & out) {
	float iw = in.cols;
	float ih = in.rows;
	if(iw > wWidth) {
		float ratio = wWidth/ iw;
		iw = iw * ratio;
		ih = ih * ratio;
	} else {
		if(ih > wHeight) {
			float ratio = wHeight/ ih;
			iw = iw * ratio;
			ih = ih * ratio;
		}
	}
	auto deltaW = (wWidth - iw)/2;
	auto deltaH = (wHeight - ih)/2;
	resize(in, out, cv::Size((int)iw, (int)ih), 0, 0, INTER_CUBIC);
	copyMakeBorder(out, out, deltaH, deltaH, deltaW, deltaW, BORDER_CONSTANT, 0);
}

Mat dBuffer;

void showFrame(Mat & cgBuffer)
{
	resizeKeepAspectRatio(gWidth, gHeight, cgBuffer, dBuffer);
	imshow("Display window", dBuffer);
}
