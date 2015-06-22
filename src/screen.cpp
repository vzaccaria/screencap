#include <iostream>
#include <utility>
#include <unistd.h>
#include "osx.hpp"
#include "ocv.hpp"
#include "opts.hpp"
#include "print.hpp"
#include "cppformat/format.h"

using namespace std;
using namespace cv;
using namespace teasy;
using namespace fmt;


/*
  10ms = 10000us -> 70% cpu
  100ms -> 40%;
  200ms -> 7%;
*/

#define MS(n)    (n * 1000)

cv::Mat cgBuffer;

int main(int argc, const char **argv)
{

	auto opts = getOpts(argc, argv);
	auto wid = getWindowID(opts.program);
	printHeader();

	if(wid != 0) {
		initFrame(opts.width, opts.height);
		while (true) {
			usleep(MS(200));
			auto windowImage = getWindowImage(wid);
			convertImage(windowImage,cgBuffer);
			showFrame(cgBuffer);
		}
		waitKey(0);
	} else {
		reportError(TS_CANT_FIND_PROGRAM, fmt::format("Cannot find window for `{}`", opts.program));
	}
}
