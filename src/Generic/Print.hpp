#include <string>

#define TS_CANT_FIND_PROGRAM 1
#define TS_CANT_READ_CONFIG  2

extern void reportMessage(const std::string & message);
extern void reportError(int code, const std::string & message);
extern void printHeader();
