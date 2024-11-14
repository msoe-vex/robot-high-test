
#include<iostream>
#include "pros/adi.hpp"
class OdomThing {
public:
    const pros::ADIEncoder* enc;

    OdomThing(std::int8_t top, std::int8_t bot, const bool reverse = false) {
        enc = new pros::ADIEncoder(top, bot, reverse);
    }

    std::int32_t get_value() {
        return 1;
    }

    ~OdomThing(){delete enc;}

};