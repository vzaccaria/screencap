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


typedef decltype(executeViewCommand) trampoline;


bool justExit(vector<string> const & v) {
	exit(0);
}

map<string, string> aliases;
map<string, trampoline *> tramp = {
	{ string("view"), executeViewCommand },
	{ string("exit"), &justExit }
};


void processString(const string & s) {
	auto w = _s::words(s);
	auto n = w[0];
	
	if(tramp.count(n)) {
		w.erase(w.begin());
		tramp[n](w);
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
	thread([=]{
			commandLoop();
		}).detach();
}

