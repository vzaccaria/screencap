#include <unistd.h>
#include <iostream>
#include <thread>
#include <map>
#include "underscore.hxx"
#include "Commands/View.hpp"
#include "cppformat/format.h"
#include "linenoise/linenoise.h"
#include "debug.hxx"


using namespace std;

std::map<string, string> aliases;




void processString(const string & s) {
	auto w = _s::words(s);
	if(w[0] == "view") {
		w.erase(w.begin());
		executeViewCommand(w);
	}

	if(w[0] == "exit") {
		exit(0);
	}
}

void commandLoop() {
	char *l;
	do {
		l = linenoise("> ");
		processString(l);
		linenoiseHistoryAdd(l);
		free(l);
	} while(l != NULL);
}

void startCommandLoop() {
	std::thread([=]{
			commandLoop();
		}).detach();
}

