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

void videoLoop() {
	while (true) {
		auto state = getState();
		if(state.displayOpen && state.currentProgram) {
			processFrame(state.currentProgram);
		}
		waitKey(300);
	}	
}


int main(int argc, const char **argv)
{
	printHeader();
	startCommandLoop();
	videoLoop();
}
