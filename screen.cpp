#include "CF++/include/CF++.h"
#include <CoreGraphics/CoreGraphics.h>
#include <Carbon/Carbon.h>
#include <ImageIO/ImageIO.h>
#include <iostream>
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


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

int main(int argc, char const* argv[])
{

	namedWindow( "Display window", WINDOW_AUTOSIZE );
	auto windowList = __a(CGWindowListCopyWindowInfo(wOptionAll | wOnScreenOnly, wNullWindow));

	for (auto window: windowList) {
		auto name = __s(__dv(window, _s("kCGWindowOwnerName")));
		if (name == "Atom") {
			auto windowId    = __n(__dv(window, _s("kCGWindowNumber")));
			auto windowImage = CGWindowListCreateImage(_wrect, _wlopts, windowId, _wcopts);
			writeImage(windowImage, "./prova.png");
		}
	}
	waitKey(0);                                          // Wait for a keystroke in the window
}
// int GetMacSpacesID()
// {
//     int currentSpace = 0;
//
// #ifdef __WXMAC__
//      // get an array of all the windows in the current Space
//     CFArrayRef windowsInSpace = CGWindowListCopyWindowInfo(kCGWindowListOptionAll | kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
//
//     // now loop over the array looking for a window with the kCGWindowWorkspace key
//     for (NSMutableDictionary *thisWindow in (NSArray *)windowsInSpace) {
//         if ([thisWindow objectForKey:(id)kCGWindowWorkspace]) {
//             currentSpace = [[thisWindow objectForKey:(id)kCGWindowWorkspace] intValue];
//             break;
//         }
//     }
// #endif
//
//     return currentSpace;
// }
