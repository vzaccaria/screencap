#include "ocv.hpp"
#include <algorithm>

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

template<typename Func>
Mat overlayOn(Mat & out, float opacity, Func lam) {
	Mat in;
	out.copyTo(in);
	lam(in);
	addWeighted(in, opacity, out, 1.0 - opacity, 0.0, out);
	return out;
}

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
	auto deltaW = std::max(0.0f,(wWidth - iw)/2);
	auto deltaH = std::max(0.0f,(wHeight - ih)/2);
	resize(in, out, cv::Size((int)iw, (int)ih), 0, 0, INTER_CUBIC);
	copyMakeBorder(out, out, deltaH, deltaH, deltaW, deltaW, BORDER_CONSTANT, 0);
	out = overlayOn(out, 0.8, [=](Mat & canvas) -> void {
			circle(canvas, cv::Point(50,50),50, Scalar(255,255,255),CV_FILLED, 8,0);
		});
}

Mat dBuffer;

void showFrame(Mat & cgBuffer)
{
	resizeKeepAspectRatio(gWidth, gHeight, cgBuffer, dBuffer);
	imshow("Display window", dBuffer);
}
