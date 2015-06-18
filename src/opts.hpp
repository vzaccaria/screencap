#include <string>

namespace teasy {
	typedef struct opts {
		std::string  program;
		unsigned int width;
		unsigned int height;
    } opts;

	opts getOpts(int argc, const char** argv);
}

