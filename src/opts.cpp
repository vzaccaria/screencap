#include "docopt/docopt.h"
#include "debug.hxx"
#include "shell.hxx"
#include "json11/json11.hpp"
#include "opts.hpp"


static const char USAGE[] =
	R"(teasy.

    Usage:
      teasy conf CONFIGURATION [ -c FILE ]
      teasy direct PROGRAM [ -w WIDTH ] [ -g HEIGHT ]
      teasy (-h | --help)
      teasy (-v | --version)

    Options:
      -c, --config FILE           Configuration file
      -w, --width WIDTH           Dest window width
      -g, --height HEIGHT         Dest Window height
      -h, --help                  Show this screen
      -v, --version               Program version

)";

using namespace std;

#define checkopt(v)   (args.count(v) && args[v].isBool() && args[v].asBool())
#define checkopts(v)  (args.count(v) && args[v].isString())

teasy::opts teasy::getOpts(int argc, const char** argv)
{
    auto args = docopt::docopt(USAGE, {
			argv + 1, argv + argc
				}, true, "Teasy 0.0");

    try {
		for(auto v: args) {
			cout << v.first << ";" << v.second << endl;
		}
		if(checkopts("CONFIGURATION")) {
			string error;
			string name = args["CONFIGURATION"].asString();
			string filename = args["--config"].asString();
			auto config = shell::cat(filename);
			auto data   = (json11::Json::parse(config, error))[0][name];
			string program = data["program"].string_value();
			auto width = data["width"].int_value();
			auto height = data["height"].int_value();
			struct opts options = ((struct opts) { program, (unsigned int)width, (unsigned int)height });
			return options;
		}
    } catch(...) {
		cerr << "Error while reading configuration file" << endl;
		exit(1);
    }
}
