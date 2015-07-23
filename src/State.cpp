#include "docopt/docopt.h"
#include "debug.hxx"
#include "shell.hxx"
#include "json11/json11.hpp"
#include "cppformat/format.h"
#include "State.hpp"
#include "Generic/Print.hpp"
#include "underscore.hxx"
#include "Generic/Time.hpp"
#include <vector>

using namespace std;
using namespace _;
using namespace teasy;

std::mutex curStateAccess;
state curState;

void initState() {
	std::lock_guard<std::mutex> guard(curStateAccess);
	curState = { false, false, false, {}, 0, 0, 0, {} };
}

state getState() {
	std::lock_guard<std::mutex> guard(curStateAccess);
	return curState;
}

