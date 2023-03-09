#include "g_rand.h"

extern int i_rand(int min, int max) {
	std::srand(std::time(nullptr));
	return (std::rand() % (max + 1)) + min;
}

extern bool b_rand() {
	std::srand(std::time(nullptr));
	return std::rand() % 2;
}