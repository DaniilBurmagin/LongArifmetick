#pragma once

#include <stdexcept>

class DivZero : public std::exception {
public:
    const char* what() const noexcept;
};
