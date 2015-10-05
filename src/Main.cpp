#include <iostream>
#include <utility>
#include <unistd.h>
#include "Draw.hpp"
#include "State.hpp"
#include "Generic/Print.hpp"
#include "cppformat/format.h"
#include <folly/futures/Future.h>
#include "HAL/CoreGraphicsHelpers.hpp"
#include "Cli.hpp"
#include "debug.hxx"
#include "docopt/docopt.h"
#include "Commands/Helpers.hpp"
#include "underscore.hxx"


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


Future<void> processFrame(CGWindowID wid) {
	auto promise = make_shared<Promise<void>>(); /* Shared is for counting references to it. */
	std::thread([=]{
			cv::Mat cgBuffer;
			auto windowImage = getWindowImage(wid);
			convertImage(windowImage,cgBuffer);
			showFrame(cgBuffer);			
			promise->setValue();
		}).detach();
	return promise->getFuture();
}

#define SLP 300

static const char USAGE[] =
	R"(teasy.

    Usage:
      teasy eval EXPR 
      teasy [ -h | --help ]

    Commands:
      eval                        Evaluate an expression as if it were typed on the command line

    Arguments:
      EXPR                        Lines should be separated by semicolons (;)

)";

void parseOptions(int argc, const char **argv) {
	
	auto args = docopt::docopt(USAGE, {
			argv + 1, argv + argc
				}, true, "Teasy 0.0");

	if(__is_true("eval")) {
		auto commandList = __s_either_or("EXPR", "");
		auto cm = _s::words(commandList, ";");
		for(auto s: cm) {
			processString(s);
		}
	}
}




int main(int argc, const char **argv)
{
	printHeader();
	parseOptions(argc, argv);
	startCommandLoop();
	while(true) {
		auto state = getState();
		initFrame(state.width, state.height);
		if(state.displayOpen) {
            if(state.currentProgram) {
			 	processFrame(state.currentProgram);
            }
			auto c = waitKey(SLP);
			debugf("we got {}", c);
            if(c != -1) {
                processChar(c);
            }
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(SLP));
		}
	}
}
