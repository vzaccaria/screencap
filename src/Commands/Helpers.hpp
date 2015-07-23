#ifndef _HELPERS_H_
#define _HELPERS_H_

#define __is_true(v)   (args.count(v) && args[v].isBool() && args[v].asBool())

#define __s_either_or(v, default) (((args.count(v) > 0) && args[v].isString()) ? args[v].asString() : default)
#define __i_either_or(v, default) (((args.count(v) > 0) && args[v].isString()) ? std::stoi(args[v].asString()) : default)
#define __b_either_or(v, default) (((args.count(v) > 0) && args[v].isBool()) ? args[v].asBool() : default)

#endif /* _HELPERS_H_ */
