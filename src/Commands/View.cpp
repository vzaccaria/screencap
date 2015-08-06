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

typedef struct sInfo {
	int width;
	int height;
} sInfo;
	
typedef std::map<std::string, sInfo> sInfos;

sInfos sInfosValues = {
	{"vga", {640, 480}},
	{"svga", {800, 600}},
	{"xga", {1024, 768}},
	{"sxga", {1280, 1024}}, 
	{"wxga", {1280, 768}},
	{"uxga", {1600, 1200}},
	{"2k", {2048, 1080}},
	{"4k", {4096, 3072}}
};

void showSInfosValues() {
	for(auto x: sInfosValues) {
		std::cout << fmt::format("{:<10} - {:>4} x {:<4}", x.first, x.second.width, x.second.height) << std::endl;
	}
}

static const char USAGE[] =
					 R"(view.

    Usage:
      view init (NAME | (-l | --list))
      view (next | previous | toggle)
      view set NUM
      view list 
      view help

    Arguments:
      NUM                         Index of the program to show (from `view list`)

    Commands:
      init                        Open window with the specified dimensions
      set                         Insert PROGRAM into the vis. stack
      next                        Show next program in the vis. stack
      previous                    Show prev program in the vis. stack
      toggle                      Toggle vis. between last two programs
      list                        Display a list of available windows
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

void listOfWindows() {
	auto l = getWindowList();
	for(auto i=0; i < l.size(); i++) {
		auto w = l[i];
		cout << fmt::format("{} - {:<40} ({:x})", i, w.first, w.second) << endl;
	}
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
			if(__is_true("--list") || __is_true("-l")) {
				showSInfosValues();
			} else {
				auto sn = __s_either_or("NAME", "small");
				if(sInfosValues.count(sn)) {
					modifyState([&](teasy::state *s) {
							s->width = sInfosValues[sn].width;
							s->height = sInfosValues[sn].height;
							s->displayOpen = true;
						});					
				} else {
					reportError(TS_COMMAND_ERROR, "Invalid configuration specified");
					return false;
		
				}
			}
		}


		if(__is_true("list")) {
			listOfWindows();
		}

		if(__is_true("set")) {
			auto p = __i_either_or("NUM", 0);
			auto l = getWindowList();
			if(p < l.size()) {
				auto wid = l[p].second;
				if(wid != 0) {
					modifyState([&](teasy::state *s) {
							s->currentProgram = wid;
							s->programQueue.push_front(wid);
						});
				}
			} else {
				reportError(TS_COMMAND_ERROR, "Invalid program specified");
				return false;
			}
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


