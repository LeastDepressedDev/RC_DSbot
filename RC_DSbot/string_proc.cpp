#include "string_proc.h"

bool str_proc::start_with(std::string s1, std::string s2) {
	int n = std::min(s1.length(), s2.length());
	for (int i = 0; i < n; i++) {
		if (s1[i] != s2[i]) {
			return false;
		}
	}
	return true;
}

bool str_proc::contains(std::string str, std::string segment) {
	int aug = 0;
	for (int i = 0; i < str.length(); i++) {
		if (segment[aug] == str[i]) {
			aug++;
		}
		else {
			aug = 0;
		}

		if (aug == segment.length()) {
			return true;
		}
	}
	return false;
}