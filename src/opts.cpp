#include "docopt/docopt.h"
#include "debug.hxx"
#include "shell.hxx"
#include "json11/json11.hpp"
#include "cppformat/format.h"
#include "opts.hpp"
#include "print.hpp"
#include "underscore.hxx"
#include "time.hpp"
#include <vector>

static const char USAGE[] =
	R"(teasy.

    Usage:
      teasy PROGRAM  [ -c CFG ] [ -f FILE ]
      teasy (-h | --help)
      teasy (-v | --version)

    Options:
      -c, --config CFG            Configuration to choose,    [default]
      -f, --file FILE             Configuration file          [config.json]
      -h, --help                  Show this screen
      -v, --version               Program version

)";

using namespace std;
using namespace _;

#define checkopt(v)   (args.count(v) && args[v].isBool() && args[v].asBool())
#define checkopts(v)  (args.count(v) && args[v].isString())

#define getOptString(v, default) (((args.count(v) > 0) && args[v].isString()) ? args[v].asString() : default)
#define getOptBool(v, default)   (((args.count(v) > 0) && args[v].isBool()) ? args[v].asBool() : default)

auto jGetInt = lambda(Json obj, string field) {
	if(!obj[field].is_number()) {
		throw fmt::format("Sorry, field {} is expected to be integer", field);
	} else {
		return obj[field].int_value();
	}
};


teasy::opts teasy::getOpts(int argc, const char** argv)
{
    auto args = docopt::docopt(USAGE, {
			argv + 1, argv + argc
				}, true, "Teasy 0.0");

    try {
		for(auto v: args) {
			debugm(v.first);
			if(v.second.isString()) {
				debugm(v.second.asString());
			}
		}
		string error;
		string name = getOptString("--config", "default");
		string filename = getOptString("--file", "config.json");
			
		auto config = shell::cat(filename);
		auto data   = (json11::Json::parse(config, error))[0][name];
			
		string program = args["PROGRAM"].asString();

		reportMessage("Using filename " + filename);
		reportMessage("Using configuration " + name);
			
		auto width = data["width"].int_value();
		auto height = data["height"].int_value();
		vector<time_t> timepoints;
		if(data["timepoints"].is_array()) {
			forEach(data["timepoints"], [&](Json val) -> void {
					auto h = jGetInt(val, "hours");
					auto m = jGetInt(val, "minutes");
					debugm(fmt::format("detected hour: {}, min: {}", h, m));
					const time_t t = todayAt(h, m);
					timepoints.push_back(t);
				});
		}
		sort(timepoints.begin(), timepoints.end());
		struct opts options = ((struct opts) { program, (unsigned int)width, (unsigned int)height, timepoints });
		return options;
    } catch(...) {
		reportError(TS_CANT_READ_CONFIG, "Cannot read configuration file.");
    }
    exit(1);
}
