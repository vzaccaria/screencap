#include "docopt/docopt.h"
#include "shell.hxx"
#include "debug.hxx"
#include "json11/json11.hpp"
#include "opts.hpp"

using namespace teasy;

static const char USAGE[] =
R"(teasy.

    Usage:
      teasy conf CONFIGURATION [ -c FILE ]
      teasy direct PROGRAM [ -w WIDTH ] [ -h HEIGHT ]
      teasy (-h | --help)
      teasy (-v | --version)

    Options:
      -c, --config FILE           Configuration file
      -w, --width WIDTH           Dest window width
      -h, --height HEIGHT         Dest Window height
      -h --help                   Show this screen
      -v --version                Program version

)";

using namespace std;

#define checkopt(v)   (args.count(v) && args[v].isBool() && args[v].asBool())
#define checkopts(v)  (args.count(v) && args[v].isString())

opts getOpts(int argc, const char** argv)
{
    auto args = docopt::docopt(USAGE, {
	argv + 1, argv + argc
	  }, true, "Teasy 0.0");

    try {
	if(checkopts("CONFIGURATION")) {
	  string error;
	  string name = args["CONFIGURATION"].asString();
	  auto config = shell::cat(name);
	  auto data   = json11::Json::parse(config, error);
	}
    } catch(char const *e) {
      cout << "Exception: " << e << '\n';
      cout << "Aborting" << '\n';
      exit(1);
    } catch(string &e) {
      cout << "Exception: " << e << '\n';
      cout << "Aborting" << '\n';
      exit(1);     
    }
    return 

}
