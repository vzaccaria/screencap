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

static const char USAGE[] =
	R"(view.

    Usage:
      view init WIDTH HEIGHT
      view (next | previous | toggle)
      view set PROGRAM
      view help

    Arguments:
      PROGRAM                     Program to insert into the visualization stack
      WIDTH                       Width of the displaying window
      HEIGHT                      Height of the displaying window

    Commands:
      init                        Open window with the specified dimensions
      set                         Insert PROGRAM into the vis. stack
      next                        Show next program in the vis. stack
      previous                    Show prev program in the vis. stack
      toggle                      Toggle vis. between last two programs
      help                        This help

)";

using namespace std;
using namespace _;

void debugArgs(std::map<std::string, docopt::value> & args) {
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

auto selectNext = [&](teasy::state *s) {
	auto old = s -> programQueue.front();
	s -> programQueue.pop_front();
	s -> programQueue.push_back(old);
	s -> currentProgram = s->programQueue.front();
};

auto selectPrevious = [&](teasy::state *s) {
	auto n = s -> programQueue.back();
	s -> programQueue.pop_back();
	s -> programQueue.push_front(n);
	s -> currentProgram = s->programQueue.front();
};

void toggleState() {
	modifyState([&](teasy::state *s) {
			if(s -> toggleView) {
				selectNext(s);
				s -> toggleView = false;
			} else {
				selectPrevious(s);
				s -> toggleView = true;
			}
		});
}

bool executeViewCommand(std::vector<std::string> const& argv) {
	try {
		for(auto v: argv) {
			debugm(v);
		}
		auto args = docopt::docopt_parse(USAGE, argv, false, false, false);
		
		debugArgs(args);
		
		if(__is_true("next")) {
			modifyState(selectNext);
		}
		
		if(__is_true("previous")) {
			modifyState(selectPrevious);
		}
		
		if(__is_true("toggle")) {
			toggleState();
		}

		if(__is_true("init")) {
			auto w = __i_either_or("WIDTH", 1280);
			auto h = __i_either_or("HEIGHT", 768);
			modifyState([&](teasy::state *s) {
					initFrame(w, h);
					s->width = w;
					s->height = h;
					s->displayOpen = true;
				});
		}

		if(__is_true("set")) {
			auto p = __s_either_or("PROGRAM", "NONE");
			auto wid = getWindowID(p);
			if(wid != 0) {
				modifyState([&](teasy::state *s) {
						s->currentProgram = wid;
						s->programQueue.push_front(wid);
					});
			}
		}

		if(__is_true("help")) {
			std::cout << USAGE << endl;
		}
		
	} catch(...) {
		std::cout << USAGE << endl;
		reportError(TS_COMMAND_ERROR, "Invalid arguments specified");
		return false;
	}
	return true;
}


