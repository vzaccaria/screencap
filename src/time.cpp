#include <ctime>
#include <iostream>
#include "cppformat/format.h"


using namespace std;

string computeTime(int secAdd) {
	time_t t = time(0);   // get time now
	t = t + secAdd;
    struct tm * now = localtime( & t );
	auto s = fmt::format("{}:{}", now->tm_hour, now->tm_min + 1, now->tm_mday);
	return s;
}

string curTime() {
	return computeTime(0);
}

