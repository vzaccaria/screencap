#include <iostream>
#include <utility>
#include <unistd.h>
#include "Draw.hpp"
#include "Options.hpp"
#include "Generic/Print.hpp"
#include "cppformat/format.h"
#include <folly/futures/Future.h>
#include "HAL/CoreGraphicsHelpers.hpp"

using namespace std;
using namespace cv;
using namespace teasy;
using namespace fmt;
using namespace folly;

/*
  10ms = 10000us -> 70% cpu
  100ms -> 40%;
  200ms -> 7%;
*/


Future<void> processFrame(CGWindowID wid, const teasy::opts &o) {
	auto promise = make_shared<Promise<void>>(); /* Shared is for counting references to it. */
	std::thread([=]{
			cv::Mat cgBuffer;
			auto windowImage = getWindowImage(wid);
			convertImage(windowImage,cgBuffer);
			showFrame(cgBuffer, o);			
			promise->setValue();
		}).detach();
	return promise->getFuture();
}

void parseKey(char k, teasy::opts &o) {
	if(k=='t') {
		o.showJustTime = ! o.showJustTime;
	}
}


int main(int argc, const char **argv)
{

	auto opts = getOpts(argc, argv);
	auto wid = getWindowID(opts.program);
	printHeader();

	if(wid != 0) {
		initFrame(opts.width, opts.height);
		while (true) {
			processFrame(wid, opts);
			auto k = waitKey(300);
			parseKey(k, opts);
		}
	} else {
		reportError(TS_CANT_FIND_PROGRAM, fmt::format("Cannot find window for `{}`", opts.program));
	}
}
