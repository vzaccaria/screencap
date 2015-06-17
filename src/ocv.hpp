#include "CF++.h"
#include <CoreGraphics/CoreGraphics.h>
#include <Carbon/Carbon.h>
#include <ImageIO/ImageIO.h>
#include <iostream>
#include <opencv2/opencv.hpp>

extern void convertImage(CGImageRef imageRef, cv::Mat & cgBuffer);
extern void initFrame();
extern void showFrame(cv::Mat & cgBuffer);
