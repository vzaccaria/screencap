#include "CF++.h"
#include <CoreGraphics/CoreGraphics.h>
#include <Carbon/Carbon.h>
#include <ImageIO/ImageIO.h>
#include <iostream>
#include <utility>
#include <opencv2/opencv.hpp>
#include <unistd.h>

CGWindowID getWindowID(std::string wname);
CGImageRef getWindowImage(CGWindowID windowId);
