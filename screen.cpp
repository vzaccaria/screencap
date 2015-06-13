#include "CF++/include/CF++.h"
#include <Carbon/Carbon.h>
#include <iostream>
#include <utility>

auto wOptionAll    = kCGWindowListOptionAll;
auto wOnScreenOnly = kCGWindowListOptionOnScreenOnly;
auto wNullWindow   = kCGNullWindowID;

using namespace std;

#define  __a(x)        (CF::Array(x).GetValues())
#define  __d(x)        (CF::Dictionary(x).GetKeysAndValues())
#define  __dv(x, k)    (CF::Dictionary(x).GetValue(k))
#define  __s(x)        (CF::String(x).GetValue())
#define  __n(x)        (CF::Number(x).GetSInt64Value())
#define  _s(x)         (CF::String(x).GetCFObject())

int main(int argc, char const* argv[])
{
	auto windowList = __a(CGWindowListCopyWindowInfo(wOptionAll | wOnScreenOnly, wNullWindow));

	for (auto window: windowList) {
		auto name   = __dv(window, _s("kCGWindowOwnerName"));
		auto number = __dv(window, _s("kCGWindowNumber"));
		auto layer  = __dv(window, _s("kCGWindowLayer"));
		cout << __s(name) << " — " << __n(number) << endl;
	}
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
