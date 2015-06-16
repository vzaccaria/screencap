#include "CF++/include/CF++.h"
#include <CoreGraphics/CoreGraphics.h>
#include <Carbon/Carbon.h>
#include <ImageIO/ImageIO.h>
#include <iostream>
#include <utility>
#include <opencv2/opencv.hpp>
#include <unistd.h>



auto wOptionAll    = kCGWindowListOptionAll;
auto wOnScreenOnly = kCGWindowListOptionOnScreenOnly;
auto wNullWindow   = kCGNullWindowID;

using namespace std;
using namespace cv;

#define  __a(x)        (CF::Array(x).GetValues())
#define  __d(x)        (CF::Dictionary(x).GetKeysAndValues())
#define  __dv(x, k)    (CF::Dictionary(x).GetValue(k))
#define  __s(x)        (CF::String(x).GetValue())
#define  __n(x)        (CF::Number(x))
#define  _s(x)         (CF::String(x).GetCFObject())

auto _wlopts = kCGWindowListOptionIncludingWindow;
auto _wrect  = CGRectNull;
auto _wcopts = kCGWindowImageBoundsIgnoreFraming;

bool writeImage(CGImageRef windowImage, string path)
{
	auto fileName = (CF::URL::FileSystemURL(path));
	auto dest     = CGImageDestinationCreateWithURL(fileName, kUTTypePNG, 1, NULL);

	CGImageDestinationAddImage(dest, windowImage, NULL);
	return CGImageDestinationFinalize(dest);
}



Mat cgBuffer;
Mat imBuffer;


void convertImage(CGImageRef imageRef)
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



void showFrame()
{
	resize(cgBuffer, cgBuffer,  cv::Size(), 0.8, 0.8, INTER_CUBIC);
	imshow("Display window", cgBuffer);
}

/*
        10ms = 10000us -> 70% cpu
        100ms -> 40%;
                                200ms -> 7%;
 */

#define MS(n)    (n * 1000)


int main(int argc, char const* argv[])
{
	#define sizex    500
	#define sizey    900


	initFrame();


	auto windowList = __a(CGWindowListCopyWindowInfo(wOptionAll | wOnScreenOnly, wNullWindow));

	for (auto window: windowList) {
		auto name = __s(__dv(window, _s("kCGWindowOwnerName")));
		if (name == "Atom") {
			auto windowId = __n(__dv(window, _s("kCGWindowNumber")));
			while (true) {
				usleep(MS(200));
				auto windowImage = CGWindowListCreateImage(_wrect, _wlopts, windowId, _wcopts);
				convertImage(windowImage);
				showFrame();
			}
		}
	}
	waitKey(0);                                          // Wait for a keystroke in the window
}
