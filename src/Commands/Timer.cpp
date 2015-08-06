#include "docopt/docopt.h"
#include "debug.hxx"
#include "shell.hxx"
#include "json11/json11.hpp"
#include "cppformat/format.h"
#include "../Generic/Print.hpp"
#include "../State.hpp"
#include "underscore.hxx"
#include "../Generic/Time.hpp"
#include <vector>
#include "Helpers.hpp"
#include "../Draw.hpp"
#include "map"

static const char USAGE[] =
	R"(timer.

    Usage:
      timer set HH MM
      timer clear
      timer show
      timer display
      timer help

    Arguments:
      HH                          Hours
      MM                          Minutes

    Commands:
      set                         set a new timer
      clear                       clear all timers
      show                        show all timers
      display                     toggle show only time
      help                        This help

)";

using namespace std;
using namespace _;

static void debugArgs(std::map<std::string, docopt::value> & args) {
	auto m = std::string("");
	for(auto v: args) {
		if(v.second.isString()) {
			m = fmt::format("{} = {}", v.first, v.second.asString());
		} else if(v.second.isBool()) {
			m = fmt::format("{} = {}", v.first, v.second.asBool());
		} else {
			
			m = fmt::format("{} = unknown", v.first);
		}
		debugm(m);
	}
}



bool executeTimerCommand(std::vector<std::string> const& argv) {
	try {
		for(auto v: argv) {
			debugm(v);
		}
		auto args = docopt::docopt_parse(USAGE, argv, false, false, false);
		
		debugArgs(args);
		
		if(__is_true("set")) {
			auto hh = __i_either_or("HH", 0);
			auto mm = __i_either_or("MM", 0);
			modifyState([&](teasy::state *s) {
					s->timepoints.push_back(todayAt(hh, mm));
					sort(s->timepoints.begin(), s->timepoints.end());
				});
		}
		
 		if(__is_true("show")) {
			auto s = getState();
			for(auto t: s.timepoints) {
				std::cout << fmt::format("- {}", time2s(t)) << std::endl;
			}
		}

		if(__is_true("clear")) {
			modifyState([&](teasy::state *s) {
					s->timepoints.clear();
				});
		}

		if(__is_true("display")) {
			modifyState([&](teasy::state *s) {
					s->showJustTime = !s->showJustTime;
				});
		}

		if(__is_true("help")) {
			std::cout << USAGE << endl;
		}
		
	} catch(const std::exception& ex) {
		debugf("Exception message: {}", ex.what());
		std::cout << USAGE << endl;
		reportError(TS_COMMAND_ERROR, "Invalid arguments specified");
		return false;
	}
	return true;
}


