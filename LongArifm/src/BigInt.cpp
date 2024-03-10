#include "../include/BigInt.hpp"

namespace big_int {

    const uint32_t BigInt::BASE_ = 1'000'000'000;

    BigInt::BigInt() : number_(1, 0), positive_(true), pres_(0) {}

    BigInt::BigInt(int32_t num, size_t pres = 0) : number_(1, std::abs(num)), positive_(num >= 0) {}

    void BigInt::ReverseNumber() {
        for (int i = 0; i < number_.size() / 2; i++) {
            std::swap(number_[i], number_[number_.size() - i - 1]);
        }
    }

    uint32_t BigInt::CountDigit(uint32_t num) const {
        if (num == 0) {
            return 1;
        }
        uint32_t count = 0;
        while (num > 0) {
            num /= 10;
            count++;
        }
        return count;
    }

    BigInt::BigInt(const std::string& str) {
        size_t idx = str.find('.');
        int pres;
        if (idx == std::string::npos) {
            pres = str.size();
        }
        else {
            pres = idx + 1;
        }
        *this = BigInt(str, str.size() - pres);
    }

    BigInt::BigInt(const std::string& str, size_t pres) {
        pres_ = pres;
        size_t frac_size = pres / 9;
        if (pres % 9 != 0) {
            ++frac_size;
        }
        fractional_.resize(frac_size, 0);
        for (size_t i = 0; i < pres / 9; ++i) {
            uint32_t value = 0;
            for (size_t j = 0; j < 9; ++j) {
                value *= 10;
                value += str[str.size() - pres] - '0';
            }
        }
        for (size_t i = 0; i < pres / 9; ++i) {
            fractional_[i] = 0;
            for (size_t j = 0; j < 9; ++j) {
                fractional_[i] *= 10;
                fractional_[i] += str[str.size() - pres+ i * 9 + j] - '0';
            }
        }
        if (pres % 9 != 0) {
            fractional_.back() = 0;
            for (size_t j = 0; j < pres % 9; ++j) {
                fractional_.back() *= 10;
                fractional_.back() += str[str.size() - pres % 9 + j] - '0';
            }
            for (size_t j = 0; j < 9 - pres % 9; ++j) {
                fractional_.back() *= 10;
            }
        }

        size_t start = 0;

        if (str[0] == '-') {
            positive_ = false;
            start++;
        }
        else if (str[0] == '+') {
            positive_ = true;
            start++;
        }
        else {
            positive_ = true;
        }
        size_t size_of_part;
        if (pres != 0) {
            size_of_part = str.size() - pres - 1;
        }
        else {
            size_of_part = str.size();
        }
        for (size_t i = 0; i < (size_of_part - start) / 9; ++i) {
            number_.push_back(0);
            for (size_t j = 0; j < 9; j++) {
                number_.back() *= 10;
                number_.back() += str[size_of_part - (i + 1) * 9 + j] - '0';
            }
        }
        if ((size_of_part - start) % 9 != 0) {
            number_.push_back(0);
            for (size_t i = 0; i < (size_of_part - start) % 9; ++i) {
                number_.back() *= 10;
                number_.back() += str[start + i] - '0';
            }
        }
    }

    std::string BigInt::ToString() const {
        std::string result;
        if (!positive_) {
            result.push_back('-');
        }
        result += std::to_string(number_.back());

        // ����� �����

        for (size_t i = number_.size() - 1; i > 0; i--) {
            uint32_t len;
            len = CountDigit(number_[i - 1]);
            for (int j = 0; j < 9 - len; j++) {
                result += '0';
            }
            result += std::to_string(number_[i - 1]);
        }
        // ������� �����
        if (pres_ == 0) {
            return result;
        }

        result.push_back('.');
        for (size_t i = 0; i + 1 < fractional_.size(); ++i) {
            uint32_t len;
            len = CountDigit(fractional_[i]);
            for (size_t j = 0; j < 9 - len; j++) {
                result += '0';
            }
            result += std::to_string(fractional_[i]);
        }
        if (fractional_.empty()) {
            return result;
        }
        uint32_t last = fractional_.back();
        size_t pres = pres_ % 9;
        if (pres == 0) {
            pres = 9;
        }
        for (size_t i = 0; i < 9 - pres; ++i) {
            last /= 10;
        }
        uint32_t len;
        len = CountDigit(last);
        for (size_t j = 0; j < pres - len; j++) {
            result += '0';
        }
        result += std::to_string(last);
        return result;
    }


    bool BigInt::AbsLess(const BigInt& other) const {
        if (number_.size() > other.number_.size()) {
            return false;
        }
        if (number_.size() < other.number_.size()) {
            return true;
        }
        for (size_t i = number_.size(); i > 0; i--) {
            if (number_[i - 1] > other.number_[i - 1]) {
                return false;
            }
            if (number_[i - 1] < other.number_[i - 1]) {
                return true;
            }
        }
        size_t min;
        if (fractional_.size() > other.fractional_.size()) {
            min = other.fractional_.size();
        }
        else {
            min = fractional_.size();
        }
        for (size_t i = 0; i < min; i++) {
            if (fractional_[i] > other.fractional_[i]) {
                return false;
            }
            if (fractional_[i] < other.fractional_[i]) {
                return true;
            }
        }
        if (fractional_.size() > min) {
            for (size_t i = min; i < fractional_.size(); i++) {
                if (fractional_[i] > 0) {
                    return false;
                }
            }
        }
        else {
            for (size_t i = min; i < other.fractional_.size(); i++) {
                if (other.fractional_[i] > 0) {
                    return true;
                }
            }
        }
        return false;

    }

    BigInt BigInt::Multiply(uint32_t num, bool pos_switch, size_t switch_size) const {
        BigInt result;
        result.positive_ = true;
        if (pos_switch) {
            if (pres_ < pos_switch * 9) {
                result.pres_ = 0;
            }
            else {
                result.pres_ = pres_ - pos_switch * 9;
            }
            result.number_.resize(number_.size() + switch_size, 0);
            size_t new_frac_size = std::max(fractional_.size(), switch_size) - switch_size;
            result.fractional_.resize(new_frac_size);
            uint64_t add = 0;
            uint64_t sum = 0;
            for (size_t i = new_frac_size; i > 0; i--) {
                sum = fractional_[i + switch_size - 1];
                sum *= num;
                sum += add;
                result.fractional_[i - 1] = sum % BASE_;
                add = sum / BASE_;
            }
            for (size_t i = fractional_.size() - new_frac_size; i > 0; i--) {
                sum = fractional_[i - 1];
                sum *= num;
                sum += add;
                result.number_[switch_size - i] = sum % BASE_;
                add = sum / BASE_;
            }
            for (size_t i = 0; i < number_.size(); i++) {
                sum = number_[i];
                sum *= num;
                sum += add;
                result.number_[switch_size + i] = sum % BASE_;
                add = sum / BASE_;
            }
            if (result.number_.back() == 0) {
                result.number_.back() += add;
                add /= BASE_;
            }
            if (add != 0) {
                result.number_.push_back(add);
            }
            while (result.number_.size() > 1 && result.number_.back() == 0) {
                result.number_.pop_back();
            }
            return result;
        }
        result.pres_ = pres_ + switch_size * 9;
        if (number_.size() > switch_size) {
            result.number_.resize(std::max(number_.size() - switch_size, size_t(1)), 0);
        }
        size_t new_frac_size = fractional_.size() + switch_size;
        result.fractional_.resize(new_frac_size, 0);
        uint32_t add = 0;
        uint64_t sum = 0;
        for (size_t i = fractional_.size(); i > 0; i--) {
            sum = fractional_[i - 1];
            sum *= num;
            sum += add;
            result.fractional_[i - 1 + switch_size] = sum % BASE_;
            add = sum / BASE_;
        }
        for (size_t i = 0; i < std::min(switch_size, number_.size()); ++i) {
            sum = number_[i];
            sum *= num;
            sum += add;
            result.fractional_[switch_size - i - 1] = sum % BASE_;
            add = sum / BASE_;
        }
        for (size_t i = switch_size; i < number_.size(); i++) {
            sum = number_[i];
            sum *= num;
            sum += add;
            result.number_[i - switch_size] = sum % BASE_;
            add = sum / BASE_;
        }
        if (result.number_.empty()) {
            result.number_.push_back(0);
        }
        if (add != 0) {
            if (switch_size > number_.size()) {
                result.fractional_[switch_size - number_.size() - 1] = add;
            }
            else {
                if (result.number_.back() == 0) {
                    result.number_.back() += add;
                    add /= BASE_;
                }
                else {
                    result.number_.push_back(add);
                }
            }
        }


        while (result.number_.size() > 1 && result.number_.back() == 0) {
            result.number_.pop_back();
        }
        return result;
    }

    void BigInt::Divide_2() {
        uint32_t add = 0;
        for (size_t i = number_.size(); i > 0; i--) {
            uint32_t cur = (number_[i - 1] + add * BASE_);
            number_[i - 1] =  cur / 2;
            add = cur % 2;
        }
        for (size_t i = 0; i < fractional_.size(); i++) {
            uint32_t cur = (fractional_[i] + add * BASE_);
            fractional_[i] = cur / 2;
            add = cur % 2;
        }
        if (add != 0) {
            fractional_.push_back(add);
            fractional_.back() *= 10;
            fractional_.back() /= 2;
            while (fractional_.back() < BASE_ / 10) {
                fractional_.back() *= 10;
            }
        }
        pres_ = fractional_.size() * 9;
    }

    BigInt BigInt::AbsDiff(const BigInt& other) const {
        BigInt num1 = *this;
        BigInt num2 = other;
        num1.positive_ = true;
        num2.positive_ = true;
        return num1 - num2;
    }

    BigInt& BigInt::operator+=(const BigInt& other) {
        pres_ = std::max(pres_, other.pres_) + 15;
        if (positive_ == other.positive_) {
            std::vector<uint32_t> new_frac(std::max(fractional_.size(), other.fractional_.size()));
            uint32_t add = 0;
            size_t size;
            if (fractional_.size() > other.fractional_.size()) {
                for (size_t i = fractional_.size(); i > other.fractional_.size(); i--) {
                    new_frac[i - 1] = fractional_[i - 1];
                }
                for (size_t i = other.fractional_.size(); i > 0; i--) {
                    uint32_t sum = 0;
                    sum = fractional_[i - 1] + other.fractional_[i - 1] + add;
                    new_frac[i - 1] = sum % BASE_;
                    add = sum / BASE_;
                }
            }
            else {
                for (size_t i = other.fractional_.size(); i > fractional_.size(); i--) {
                    new_frac[i - 1] = other.fractional_[i - 1];
                }
                for (size_t i = fractional_.size(); i > 0; i--) {
                    uint32_t sum = 0;
                    sum = other.fractional_[i - 1] + fractional_[i - 1] + add;
                    new_frac[i - 1] = sum % BASE_;
                    add = sum / BASE_;
                }
            }
            fractional_ = new_frac;

            if (number_.size() >= other.number_.size()) {
                uint32_t sum = 0;
                for (size_t i = 0; i < other.number_.size(); i++) {
                    sum = number_[i] + other.number_[i] + add;
                    number_[i] = sum % BASE_;
                    add = sum / BASE_;
                }
                for (size_t i = other.number_.size(); i < number_.size(); i++) {
                    if (add == 0) {
                        break;
                    }
                    sum = number_[i] + add;
                    number_[i] = sum % BASE_;
                    add = sum / BASE_;
                }
                if (add != 0) {
                    number_.push_back(add);
                }
                return *this;
            }
            size = number_.size();
            number_.resize(other.number_.size());
            uint32_t sum = 0;
            for (size_t i = 0; i < size; i++) {
                sum = number_[i] + other.number_[i] + add;
                number_[i] = sum % BASE_;
                add = sum / BASE_;
            }
            for (size_t i = size; i < other.number_.size(); i++) {
                sum = other.number_[i] + add;
                number_[i] = sum % BASE_;
                add = sum / BASE_;
            }
            if (add != 0) {
                number_.push_back(add);
            }
            return *this;
        }

        BigInt different_positive_(other);
        different_positive_.positive_ = !other.positive_;
        *this -= different_positive_;
        return *this;
    }

    BigInt BigInt::operator+(const BigInt& other) const {
        return BigInt(*this) += other;
    }

    BigInt& BigInt::operator-=(const BigInt& other) {
        if (positive_ == other.positive_) {
            pres_ = std::max(pres_, other.pres_);
            size_t size;
            if (AbsLess(other)) {
                std::vector <uint32_t> copy_number = number_;
                number_ = other.number_;

                positive_ = !positive_;
                std::vector<uint32_t> copy_frac = fractional_;
                fractional_ = other.fractional_;
                if (fractional_.size() < copy_frac.size()) {
                    fractional_.resize(copy_frac.size());
                }
                for (size_t i = copy_frac.size(); i > 0; i--) {
                    if (fractional_[i - 1] >= copy_frac[i - 1]) {
                        fractional_[i - 1] -= copy_frac[i - 1];
                        continue;
                    }
                    size_t j = i - 1;
                    while (fractional_[j - 1] == 0) {
                        fractional_[j - 1] = BASE_ - 1;
                        j--;
                    }
                    if (j == 0) {
                        while (number_[j] == 0) {
                            number_[j] = BASE_ - 1;
                            ++j;
                        }
                        number_[j] -= 1;
                    }
                    else {
                        fractional_[j - 1] -= 1;
                    }
                    fractional_[i - 1] = fractional_[i - 1] + BASE_ - copy_frac[i - 1];
                }

                for (size_t i = 0; i < copy_number.size(); i++) {
                    if (number_[i] >= copy_number[i]) {
                        number_[i] -= copy_number[i];
                        continue;
                    }
                    size_t j = i + 1;
                    while (number_[j] == 0) {
                        number_[j] = BASE_ - 1;
                        j++;
                    }
                    number_[j] -= 1;
                    number_[i] = number_[i] + BASE_ - copy_number[i];
                }
                while (number_.back() == 0 && number_.size() > 1) {
                    number_.pop_back();
                }
                if (number_.back() == 0) {
                    for (size_t i = 0; i < fractional_.size(); ++i) {
                        if (fractional_[i] != 0) {
                            return *this;
                        }
                    }
                    positive_ = true;
                }
                return *this;
            }
            if (fractional_.size() < other.fractional_.size()) {
                fractional_.resize(other.fractional_.size(), 0);
            }
            for (size_t i = other.fractional_.size(); i > 0; i--) {
                if (fractional_[i - 1] >= other.fractional_[i - 1]) {
                    fractional_[i - 1] -= other.fractional_[i - 1];
                    continue;
                }
                size_t j = i - 1;
                while (j > 0 && fractional_[j - 1] == 0) {
                    fractional_[j - 1] = BASE_ - 1;
                    j--;
                }
                if (j == 0) {
                    while (number_[j] == 0) {
                        number_[j] = BASE_ - 1;
                        ++j;
                    }
                    number_[j] -= 1;
                }
                else {
                    fractional_[j - 1] -= 1;
                }
                fractional_[i - 1] = fractional_[i - 1] + BASE_ - other.fractional_[i - 1];
            }

            for (size_t i = 0; i < other.number_.size(); i++) {
                if (number_[i] >= other.number_[i]) {
                    number_[i] -= other.number_[i];
                    continue;
                }
                size_t j = i + 1;
                while (number_[j] == 0) {
                    number_[j] = BASE_ - 1;
                    j++;
                }
                number_[j] -= 1;
                number_[i] = number_[i] + BASE_ - other.number_[i];
            }

            while (number_.back() == 0 && number_.size() > 1) {
                number_.pop_back();
            }

            if (number_.back() == 0) {
                for (size_t i = 0; i < fractional_.size(); ++i) {
                    if (fractional_[i] != 0) {
                        return *this;
                    }
                }
                positive_ = true;
            }
            return *this;
        }
        BigInt different_positive_(other);
        different_positive_.positive_ = !other.positive_;
        *this += different_positive_;
        return *this;
    }

    BigInt BigInt::operator-(const BigInt& other) const {
        return BigInt(*this) -= other;
    }

    BigInt& BigInt::operator*=(const BigInt& other) {

        BigInt result;

        result.pres_ = pres_ + other.pres_;
        for (size_t i = 0; i < fractional_.size(); ++i) {
            result += other.Multiply(fractional_[i], false, i + 1);
        }
        for (size_t i = 0; i < number_.size(); ++i) {
            result += other.Multiply(number_[i], true, i);
        }
        while (result.number_.size() > 1 && result.number_.back() == 0) {
            result.number_.pop_back();
        }
        result.positive_ = (positive_ == other.positive_);
        if (result.number_.back() == 0) {
            for (size_t i = 0; i < result.fractional_.size(); ++i) {
                if (result.fractional_[i] != 0) {
                    return *this = result;
                }
            }
            result.positive_ = true;
        }
        return *this = result;
    }


    BigInt BigInt::divide(const BigInt& first, const BigInt& second, size_t divPrec) {
        if (!first.AbsLess(second) && !second.AbsLess(first)) {
            BigInt result = BigInt(1);
            result.positive_ = (first.positive_ == second.positive_);
            return result;
        }
        std::string preccision = "0.";
        for (size_t i = 0; i < divPrec; ++i) {
            preccision += '0';
        }
        preccision += '1';
        BigInt prec(preccision);
        BigInt left(0);
        BigInt right = first;
        left.positive_ = true;
        right.positive_ = true;
        while (true) {
            BigInt cur = left + right;
            cur.Divide_2();
            BigInt Mlt = cur * second;
            if (first.AbsLess(Mlt)) {
                right = cur;
                continue;
            }
            if (first.AbsLess(Mlt + prec)) {
                size_t size = divPrec / 9;
                if (divPrec % 9 != 0) {
                    ++size;
                }
                cur.fractional_.resize(size);
                if (divPrec % 9 != 0) {
                    for (size_t i = 0; i < 9 - divPrec % 9; ++i) {
                        cur.fractional_.back() /= 10;
                    }
                    for (size_t i = 0; i < 9 - divPrec % 9; ++i) {
                        cur.fractional_.back() *= 10;
                    }
                }
                cur.pres_ = divPrec;
                cur.positive_ = (first.positive_ == second.positive_);
                return cur;
            }
            left = cur;
        }
    }

    bool BigInt::operator<(const BigInt& other) const {
        if (!positive_ && other.positive_) {
            return true;
        }
        if (positive_ && !other.positive_) {
            return false;
        }
        if (positive_) {
            return AbsLess(other);
        }
        return other.AbsLess(*this);
    }

    bool BigInt::operator>(const BigInt& other) const {
        return other < *this;
    }
    bool BigInt::operator==(const BigInt& other) const {
        return !(*this < other) && !(other < *this);
    }
    bool BigInt::operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    BigInt BigInt::operator*(const BigInt& other) const {
        return BigInt(*this) *= other;
    }

}