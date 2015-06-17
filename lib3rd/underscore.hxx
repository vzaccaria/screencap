/*
Copyright (C) 2014 Vittorio Zaccaria

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef UNDERSCORE_HXX
#define UNDERSCORE_HXX

/* import dropbox json11 */
#include "json11.hpp"
#include <regex>


namespace _s {

	using namespace std;

	static string regex1(string str, string rexp) {
		/* #include <regex> */
		/* using namespace std; */
		std::regex re_str(rexp, std::regex_constants::ECMAScript | std::regex_constants::icase);
		std::smatch m_str; 
		if(std::regex_match(str, m_str, re_str)) {
			if(m_str.size() > 1) {
				return m_str[1];
			}
		} 
		return "";
	}

	static vector<string> words(string str, string delim = " ") {
		vector<string> res;
		auto pos = 0;
		while( (pos = str.find(delim)) != string::npos) {
			res.push_back(str.substr(0, pos));
			str.erase(0, pos + delim.length());
		}
		res.push_back(str);
		return res;
	}

	static string sentence(vector<string> v, string delim = " ") {
		string s = "";
		auto l = v.size();
		for(auto x=0; x<l; x++) {
			s = s + v[x]; 
			if(x != (l-1)) {
				s = s + delim;
			}
		}
		return s;
	}
}

namespace _ {

	using namespace json11;
	using namespace std;

	/* Warning, this assumes there are no two values with the same key */

	static Json indexBy(Json j1, string key) {
		Json::object ret; 
		for(const auto v: j1.array_items()) {
			auto kk = v[key].string_value();
			ret[kk] = v;
		}
		return ret;
	}

	static Json indexBy(Json j1, function<Json(Json)> fun) {
		Json::object ret; 
		for(const auto v: j1.array_items()) {
			auto kk = fun(v).string_value();
			ret[kk] = v;
		}
		return ret;
	}

	static Json filter(Json j1, function<bool(Json)> fun) {
		vector<Json> res;
		for(const auto v: j1.array_items()) {
			auto x = fun(v);
			if(x) {
				res.push_back(x);
			}
		}
		return res;		
	}

	static Json map(Json j1, function<Json(Json)> fun) {
		vector<Json> res;
		for(const auto v: j1.array_items()) {
			auto x = fun(v);
			if(!x.is_null()) {
				res.push_back(x);
			}
		}
		return res;
	}

	static Json compact(Json j1) {
		vector<Json> res;
		for(const auto v: j1.array_items()) {
			if(!v.is_null()) {
				res.push_back(v);
			}
		}
		return res;
	}

	static Json forEach(Json j1, function<void(Json)> fun) {
		for(const auto v: j1.array_items()) {
			fun(v);
		}
		return j1;
	}

	static auto jmap = map;

} // underscore

#define lambda 	[=]
#define λ      	[=]


#endif // UNDERSCORE_HXX
