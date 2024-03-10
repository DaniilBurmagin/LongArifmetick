#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdbool>
#include <string>
#include <cmath>
#include <vector>

#include "exceptions.hpp"

namespace big_int {

    class BigInt {
    private:
        std::vector<uint32_t> number_;
        bool positive_ = true;
        static const uint32_t BASE_;
        size_t pres_;
        std::vector<uint32_t> fractional_;

        bool AbsLess(const BigInt& other) const;

        BigInt Multiply(uint32_t num, bool pos_switch, size_t swich_size) const;

        BigInt AbsDiff(const BigInt& other) const;

        void Divide_2();

        void ReverseNumber();

        uint32_t CountDigit(uint32_t num) const;

    public:

        BigInt();

        explicit BigInt(int32_t num, size_t pres);

        BigInt(const std::string& str);

        std::string ToString() const;

        BigInt(const std::string& str, size_t prec);

        BigInt& operator=(const BigInt& other) = default;

        BigInt& operator+=(const BigInt& other);

        BigInt& operator-=(const BigInt& other);

        BigInt& operator*=(const BigInt& other);

        BigInt operator+(const BigInt& other) const;

        BigInt operator-(const BigInt& other) const;

        BigInt operator*(const BigInt& other) const;

        bool operator==(const BigInt& other) const;

        bool operator!=(const BigInt& other) const;

        bool operator>(const BigInt& other) const;

        bool operator<(const BigInt& other) const;

        static BigInt divide(const BigInt& first, const BigInt& second, size_t divPrec);

    };

}