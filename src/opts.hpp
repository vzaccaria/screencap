#include <string>
#include <ctime>
#include <vector>

namespace teasy {
	typedef struct opts {
		std::string    program;
		unsigned int   width;
		unsigned int   height;
		std::vector<time_t> timepoints;
    } opts;

	opts getOpts(int argc, const char** argv);
}

