#include "CoreGraphicsHelpers.hpp"
#include "cppformat/format.h"

auto wOptionAll		= kCGWindowListOptionAll;
auto wOnScreenOnly	= kCGWindowListOptionOnScreenOnly;
auto wNullWindow	= kCGNullWindowID;
auto wNoDesktop		= kCGWindowListExcludeDesktopElements;

using namespace std;

#define  __a(x)        (CF::Array(x).GetValues())
#define  __d(x)        (CF::Dictionary(x).GetKeysAndValues())
#define  __dv(x, k)    (CF::Dictionary(x).GetValue(k))
#define  __s(x)        (CF::String(x).GetValue())
#define  __n(x)        (CF::Number(x))
#define  _s(x)         (CF::String(x).GetCFObject())

auto _wlopts = kCGWindowListOptionIncludingWindow;
auto _wrect  = CGRectNull;
auto _wcopts = kCGWindowImageBoundsIgnoreFraming;

using namespace std;

CGWindowInfoList getWindowList() {
	CGWindowInfoList l;
	auto windowList = __a(CGWindowListCopyWindowInfo(wOnScreenOnly | wNoDesktop, wNullWindow));
	for (auto window: windowList) {
		auto oname = __s(__dv(window, _s("kCGWindowOwnerName")));
		auto name = __s(__dv(window, _s("kCGWindowName")));
		auto wid = __n(__dv(window, _s("kCGWindowNumber")));
		auto cname = fmt::format("{} - {}", oname, name);
		auto p = CGWindowInfo(cname, wid);
		l.push_back(p);
	}
	sort(l.begin(), l.end(), [](CGWindowInfo w1, CGWindowInfo w2) -> bool {
	 		return w1.first < w2.first;	
	 	});

	return l;
}

CGWindowID getWindowID(string wname) {
	auto windowList = __a(CGWindowListCopyWindowInfo(wOptionAll | wOnScreenOnly, wNullWindow));

	for (auto window: windowList) {
		auto name = __s(__dv(window, _s("kCGWindowOwnerName")));
		if (name == wname)
			return __n(__dv(window, _s("kCGWindowNumber")));
	}
	return NULL;
}

CGImageRef getWindowImage(CGWindowID windowId) {
	return CGWindowListCreateImage(_wrect, _wlopts, windowId, _wcopts);
}


bool writeImage(CGImageRef windowImage, string path)
{
	auto fileName = (CF::URL::FileSystemURL(path));
	auto dest     = CGImageDestinationCreateWithURL(fileName, kUTTypePNG, 1, NULL);

	CGImageDestinationAddImage(dest, windowImage, NULL);
	return CGImageDestinationFinalize(dest);
}
