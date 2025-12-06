#include<iostream>

class Power {
    public: 
        uint8_t power_active;
        uint8_t power_inactive = 0; 
        explicit Power(uint8_t power_active, uint8_t power_inactive) : power_active(power_active), power_inactive(power_inactive) {}
        explicit Power(uint8_t power_active) : power_active(power_active) {}
        friend double operator+(const Power& rhs, const Power& lhs) noexcept; 
};


int power_main_example(); 