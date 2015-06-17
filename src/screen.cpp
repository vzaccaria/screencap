#include <iostream>
#include <utility>
#include <unistd.h>
#include "osx.hpp"
#include "ocv.hpp"

using namespace std;
using namespace cv;


/*
        10ms = 10000us -> 70% cpu
        100ms -> 40%;
        200ms -> 7%;
 */

#define MS(n)    (n * 1000)

cv::Mat cgBuffer;

int main(int argc, char const* argv[])
{

	initFrame();
	auto wid = getWindowID("Terminal");
	
	while (true) {
		usleep(MS(200));
		auto windowImage = getWindowImage(wid);
		convertImage(windowImage,cgBuffer);
		showFrame(cgBuffer);
	}
	waitKey(0);                                          // Wait for a keystroke in the window
}
