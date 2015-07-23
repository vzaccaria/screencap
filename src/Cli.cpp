#include <unistd.h>
#include <iostream>
#include <thread>
#include "underscore.hxx"
#include "Commands/View.hpp"
#include "cppformat/format.h"
#include "linenoise/linenoise.h"
#include "debug.hxx"


using namespace std;



void processString(const string & s) {
	auto w = _s::words(s);
	if(w[0] == "view") {
		w.erase(w.begin());
		executeViewCommand(w);
	}
}

void commandLoop() {
	auto l = linenoise("> ");
	processString(l);
	while((l = linenoise("> ")) != NULL) {
		processString(l);
		linenoiseHistoryAdd(l);
	}
}

void startCommandLoop() {
	std::thread([=]{
			commandLoop();
		}).detach();
}

