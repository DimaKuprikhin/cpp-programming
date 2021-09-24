#include "long_integer.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

LongInteger::LongInteger() noexcept
    : sign(1)
    {}

LongInteger::LongInteger(int64_t value) noexcept
    : sign(value != 0 ? value / std::abs(value) : 1)
{
    value = std::abs(value);
    while(value) {
        data.push_back(value % base);
        value /= base;
    }
}

LongInteger::LongInteger(const std::string& number)
    : sign(1)
    , data(0)
{
    data.reserve(number.size() / 9 + 1);
    uint64_t powerOfTen = 1;
    for(int i = 0; i < (int)number.size(); ++i) {
        int stringPosition = number.size() - 1 - i;
        if(stringPosition == 0 && (number[0] == '-' || number[0] == '+')) {
            sign = number[0] == '-' ? -1 : 1;
            continue;
        }
        powerOfTen *= 10;
        if(i % 9 == 0) {
            powerOfTen = 1;
            data.push_back(0);
        }
        if(number[stringPosition] < '0' || number[stringPosition] > '9') {
            throw std::runtime_error("The given string is not a number");
        }
        data.back() += (number[stringPosition] - '0') * powerOfTen;
    }
    Trim();
}

LongInteger LongInteger::operator+() const noexcept {
    return *this;
}

LongInteger LongInteger::operator-() const noexcept {
    auto result(*this);
    result.sign *= -1;
    return result;
}

LongInteger LongInteger::operator+(const LongInteger& rhs) const noexcept {
    if(sign == rhs.sign) {
        LongInteger result;
        result.sign = sign;
        const int64_t lhsSize = data.size();
        const int64_t rhsSize = rhs.data.size();
        const int64_t resultSize = std::max(lhsSize, rhsSize);
        result.data.reserve(resultSize);
        int carry = 0;
        for(int i = 0; i < resultSize || carry; ++i) {
            const auto lhsValue = i < lhsSize ? data[i] : 0;
            const auto rhsValue = i < rhsSize ? rhs.data[i] : 0;
            auto resultValue = carry + lhsValue + rhsValue;
            if(resultValue >= base) {
                carry = 1;
                resultValue -= base;
            }
            result.data.push_back(resultValue);
        }
        return result;
    }
    return *this - (-rhs);
}

LongInteger LongInteger::operator-(const LongInteger& rhs) const noexcept {
    if(sign == rhs.sign) {
        if (Abs() >= rhs.Abs()) {
            auto result(*this);
            int carry = 0;
            for(int i = 0; i < (int)rhs.data.size() || carry; ++i) {
                const auto rhsValue = i < (int)rhs.data.size() ? rhs.data[i] : 0;
                result.data[i] -= carry + rhsValue;
                if(result.data[i] < 0) {
                    carry = 1;
                    result.data[i] += base;
                }
            }
            result.Trim();
            return result;
        }
        return -(rhs - *this);
    }
    return *this + (-rhs);
}

bool LongInteger::operator<(const LongInteger& rhs) const noexcept {
    if(IsZero() && rhs.IsZero()) {
        return false;
    }
    if(sign * rhs.sign < 0) {
        return sign < 0;
    }
    if(data.size() != rhs.data.size()) {
        return data.size() * sign < rhs.data.size() * rhs.sign;
    }
    for(int i = data.size() - 1; i >= 0; --i) {
        if(data[i] != rhs.data[i]) {
            return data[i] * sign < rhs.data[i] * rhs.sign;
        }
    }
    return false;
}

bool LongInteger::operator>(const LongInteger& rhs) const noexcept {
    return rhs < *this;
}

bool LongInteger::operator<=(const LongInteger& rhs) const noexcept {
    return !(*this > rhs);
}

bool LongInteger::operator>=(const LongInteger& rhs) const noexcept {
    return !(*this < rhs);
}

bool LongInteger::operator==(const LongInteger& rhs) const noexcept {
    return !(*this < rhs || *this > rhs);
}

LongInteger LongInteger::Abs() const noexcept {
    auto result(*this);
    result.sign *= result.sign;
    return result;
}

bool LongInteger::IsZero() const noexcept {
    return !data.size();
}

std::string LongInteger::ToString() const noexcept {
    if(IsZero()) {
        return "0";
    }
    std::stringstream result("");
    if(sign == -1) {
        result << '-';
    }
    result << std::to_string(data.back());
    for(int i = data.size() - 2; i >= 0; --i) {
        result << std::setw(9) << std::setfill('0') << data[i];
    }
    return result.str();
}

void LongInteger::Trim() noexcept {
    while(data.size() && !data.back()) {
        data.pop_back();
    }
}
