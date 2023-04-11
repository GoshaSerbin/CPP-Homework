#pragma once

#include <iostream>

struct Error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
