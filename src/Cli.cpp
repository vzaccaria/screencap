#include <unistd.h>
#include <iostream>
#include <thread>
#include <map>
#include "underscore.hxx"
#include "Commands/View.hpp"
#include "Commands/Timer.hpp"
#include "cppformat/format.h"
#include "linenoise/linenoise.h"
#include "debug.hxx"

using namespace std;

/* Docs about cppformat: http://cppformat.github.io/latest/ */

typedef decltype(executeViewCommand) trampoline;


bool justExit(vector<string> const & v) {
	debugf("Exiting..");
	exit(0);
}


bool justHelp(vector<string> const & v);

typedef std::pair<trampoline *, string> trampData;

map<string, string> aliases;

map<string, trampData> tramp = {
	{ string("view"),  { executeViewCommand , "Create and add windows by name" }},
	{ string("timer"), { executeTimerCommand, "Add and remove timer breakpoints" }},
	{ string("exit"),  { justExit           , "Exit" }},
	{ string("help"),  { justHelp           , "This help" }}
};

map<char, string> keyMap = {
	{ 'q', "exit"},
	{ 'n', "view next"},
	{ 'b', "view previous"},
	{ 'o', "view toggle"},
	{ 't', "timer display"}
};


bool justHelp(vector<string> const & v) {
	cout << "Available commands\n" << endl;
	for(auto v: tramp) {
		cout << fmt::format("{:<20} - {}", v.first, v.second.second) << endl;
	}
	cout << "\nFor help on each *command*, type '*command* help'" << endl;
	
	cout << "Available keymaps (live window)\n" << endl;
	for(auto v: keyMap) {
		cout << fmt::format("{:<20} aliased to '{}'", v.first, v.second) << endl;
	}
	return true;
}

void processString(const string & s) {
	auto w = _s::words(s);
	
	for(auto & v: w) {
		debugf("'{}'", v);
		v = _s::trim(v);
	}

	if(w.size() >= 1) {
		auto n = w[0];
	
		if(tramp.count(n)) {
			w.erase(w.begin());
			tramp[n].first(w);
		}
	}
}

void commandLoop() {
	char *l;
	do {
		l = linenoise("> ");
		debugf("read line: '{0:x}'", (long int)l);
		if(l != NULL) {
			processString(l);
			linenoiseHistoryAdd(l);
			free(l);
		}
	} while(l != NULL);
}

void startCommandLoop() {
	thread([=]{
			commandLoop();
		}).detach();
}



void processChar(char c) {
	if(keyMap.count(c)) {
		debugf("Invoking command associated with {}", c);
		processString(keyMap[c]);
	}
}
