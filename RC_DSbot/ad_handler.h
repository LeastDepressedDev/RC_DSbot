#include <iostream>


#pragma once

class Audio {
public:
	bool cor = false;
	size_t sz();
	uint8_t* sound();
	Audio(std::string path);
private:
	size_t size;
	uint8_t* arr;
};