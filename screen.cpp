#include <Carbon/Carbon.h>
#include <iostream>

auto wOptionAll     = kCGWindowListOptionAll;
auto wOnScreenOnly  = kCGWindowListOptionOnScreenOnly;
auto wNullWindow    = kCGNullWindowID;

using namespace std;

int main(int argc, char const *argv[]) {
	// auto al = CGWindowListCreate(wOptionAll | wOnScreenOnly, wNullWindow);
	// auto wlist = new core_array<CGWindowID>(al);
	// for(const auto v: wlist) {
	//      cout << wlist << endl;
	// }
	// return 0;
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
