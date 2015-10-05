#include <string>
#include <ctime>
#include <vector>
#include <deque>
#include "HAL/CoreGraphicsHelpers.hpp"

#ifndef _STATE_H_
#define _STATE_H_

namespace teasy {

	typedef struct state {
		bool  showJustTime;
		bool  displayOpen;
		bool  toggleView;
		std::deque<CGWindowID> programQueue;
		CGWindowID    currentProgram;
		unsigned int  width;
		unsigned int  height;
		std::vector<time_t> timepoints;
	} state;

}

extern std::mutex curStateAccess;
extern teasy::state curState;

extern void initState();
extern teasy::state getState();


template<typename Func>
void modifyState(Func lam) {
	std::lock_guard<std::mutex> guard(curStateAccess);
	lam(&curState);
}

#endif /* _STATE_H_ */



