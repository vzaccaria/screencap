#include <string>

#ifndef _CLI_H_
#define _CLI_H_

extern void commandLoop();

extern void startCommandLoop();

extern void processChar(char c);

extern void processString(const std::string & s);
#endif /* _CLI_H_ */
