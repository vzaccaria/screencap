#include <iostream>
#include <utility>
#include <unistd.h>
#include "HAL/CoreGraphicsHelpers.hpp"
#include "Draw.hpp"
#include "Options.hpp"
#include "Generic/Print.hpp"
#include "cppformat/format.h"
#include <folly/futures/Future.h>

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


int main(int argc, const char **argv)
{

	auto opts = getOpts(argc, argv);
	auto wid = getWindowID(opts.program);
	printHeader();

	if(wid != 0) {
		initFrame(opts.width, opts.height);
		while (true) {
			processFrame(wid, opts);
			waitKey(200);
		}
	} else {
		reportError(TS_CANT_FIND_PROGRAM, fmt::format("Cannot find window for `{}`", opts.program));
	}
}
