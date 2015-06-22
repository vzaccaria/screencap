#include <iostream>
#include "print.hpp"
#include "cppformat/format.h"
#include "config.h"

void reportError(int code, const std::string & message) {
	auto s = fmt::format("[TS {:x}] {}", code, message);
	std::cout << s << std::endl;
	exit(code);
}

void printHeader() {
	auto s = fmt::format("Teasy v.{} - (c) {} Vittorio Zaccaria\n", TS_VERSION, TS_YEAR );
	std::cout << s << std::endl;
}
