#include <string>
#include <ctime>
#include <vector>

#ifndef _OPTS_H_
#define _OPTS_H_

namespace teasy {
	typedef struct opts {
		std::string    program;
		unsigned int   width;
		unsigned int   height;
		std::vector<time_t> timepoints;
    } opts;

	opts getOpts(int argc, const char** argv);
}


#endif /* _OPTS_H_ */


