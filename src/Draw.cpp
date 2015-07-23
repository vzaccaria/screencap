#include "Draw.hpp"
#include "HAL/OpenCVHelpers.hpp"
#include <algorithm>
#include <utility>
#include <mutex>
#include <iterator>
#include "Generic/Time.hpp"

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
    
    auto o = getState();
    Mat destBuffer(gHeight, gWidth, CV_8UC4);
    std::lock_guard<std::mutex> guard(buffAccess);
    if(!o.showJustTime){
        auto displayTime = lambda(canvas) {
            overlayOn(canvas, 0.2, lambda(c2) {
                    auto rect = pad(canvas, 0.05).first;
                    drawBlackRect(c2, rect);
                });
            padCanvas(canvas, 0.1, [&](Mat & inner) {
                    auto pos = find_if(o.timepoints.begin(), o.timepoints.end(), [&](time_t v) -> bool {
                            return (v >= todayAtNow());
                        });
                    if(pos != std::end(o.timepoints)) {
                        auto df = diffTime(todayAtNow(), *pos);
                        centerText(inner, curTime() + "   " + df);
                    }
                });
        };
    
        resizeKeepAspectRatio(fromBuffer, destBuffer);
        splitV(destBuffer, 0.1, displayTime, lambda(foo) { });
    } else {
        auto pos = find_if(o.timepoints.begin(), o.timepoints.end(), [&](time_t v) -> bool {
                return (v >= todayAtNow());
            });
        if(pos != std::end(o.timepoints)) {
            auto df = diffTime(todayAtNow(), *pos);
            drawBlackRect(destBuffer, cv::Rect(0,0,destBuffer.cols,destBuffer.rows));
            centerText(destBuffer, curTime() + "   " + df, 3);
        }
    }

    imshow("Display window", destBuffer);
}
