#include <string>
#include <ctime>

extern std::string time2s(time_t);
extern std::string curTime2s();
extern time_t todayAt(int hours, int minutes);
extern time_t todayAtNow();
extern std::string diffTime(time_t s, time_t d);
