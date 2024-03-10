#include "../include/exceptions.hpp"

const char* DivZero::what() const noexcept {
    return "Ошибка! Деление на 0 невозможно";
}
