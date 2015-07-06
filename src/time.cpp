#include <ctime>
#include <iostream>
#include "cppformat/format.h"


using namespace std;

string computeTime(int secAdd) {
	time_t t = time(0);   // get time now
	t = t + secAdd;
    struct tm * now = localtime( & t );
	auto s = fmt::format("{:0>2}:{:0>2}", now->tm_hour, now->tm_min);
	return s;
}

string curTime() {
	return computeTime(0);
}

time_t todayAtNow() {
	time_t t = time(0);   // get time now
	return t;
}

string diffTime(time_t s, time_t d) {
	int df  = difftime(d, s);
	df = df / 60;
	int hr  = df / (60);
	int min = (df % 60);
	return fmt::format("{:0>2}:{:0>2}", hr, min);
}


time_t todayAt(int hours, int minutes) {
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	now -> tm_hour = hours;
	now -> tm_min = minutes;
	now -> tm_sec = 0;
	return mktime(now);
}

