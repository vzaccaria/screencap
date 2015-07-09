#include <string>
#include <ctime>

extern std::string computeTime(int secAdd);
extern std::string curTime();
extern time_t todayAt(int hours, int minutes);
extern time_t todayAtNow();
extern std::string diffTime(time_t s, time_t d);
