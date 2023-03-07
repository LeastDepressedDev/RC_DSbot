#pragma once
#include <map>
#include <iostream>

//key(id) : value(link)
namespace rsc {
	inline std::map<std::string, std::string> m_keys = std::map<std::string, std::string>();

	inline const char* PC_FNAME = "rsc.set";

	extern enum e_code : int8_t {
		rsc_loaded = 1,
		rsc_error = -2,
		rsc_not_found = -1
	};
}