#include "ad_handler.h"
#include <fstream>

Audio::Audio(std::string pth) {
    uint8_t* robot = nullptr;
    size_t robot_size = 0;
    std::ifstream input(pth, std::ios::in | std::ios::binary | std::ios::ate);
    if (input.is_open()) {
        robot_size = input.tellg();
        robot = new uint8_t[robot_size];
        input.seekg(0, std::ios::beg);
        input.read((char*)robot, robot_size);
        input.close();
        cor = true;
    }
    this->arr = robot;
    this->size = robot_size;
}

uint8_t* Audio::sound() {
    return this->arr;
}

size_t Audio::sz() {
    return this->size;
}