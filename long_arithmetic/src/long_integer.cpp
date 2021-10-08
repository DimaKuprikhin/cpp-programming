#include <long_integer.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>

LongInteger::LongInteger()
    : sign(1)
    {}

LongInteger::LongInteger(int64_t value)
    : sign(value >= 0 ? 1 : -1)
{
    while(value) {
        data.push_back(std::abs(value % base));
        value /= base;
    }
}

LongInteger::LongInteger(uint64_t value)
    : sign(1)
{
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

LongInteger::LongInteger(const LongInteger& other)
    : sign(other.sign)
    , data(other.data)
    {}

LongInteger::LongInteger(LongInteger&& other) noexcept
    : sign(other.sign)
    , data(std::move(other.data))
    {}

LongInteger& LongInteger::operator=(const LongInteger& other) {
    sign = other.sign;
    data = other.data;
    return *this;
}

LongInteger& LongInteger::operator=(LongInteger&& other) noexcept {
    sign = other.sign;
    data = std::move(other.data);
    return *this;
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
    LongInteger result = *this;
    result += rhs;
    return result;
}

LongInteger& LongInteger::operator+=(const LongInteger& rhs) noexcept {
    if(sign == rhs.sign) {
        const int64_t rhsSize = rhs.data.size();
        const int64_t size = std::max(static_cast<int64_t>(data.size()), rhsSize);
        data.resize(size);
        int carry = 0;
        for(int i = 0; i < size || carry; ++i) {
            auto value = 0;
            if(i < size) {
                value = data[i];
            } else {
                data.push_back(0);
            }
            value += carry + (i < rhsSize ? rhs.data[i] : 0);
            carry = 0;
            if(value >= base) {
                carry = 1;
                value -= base;
            }
            data[i] = value;
        }
        return *this;
    }
    // TODO: unnecessary copy.
    return *this -= (-rhs);
}

LongInteger LongInteger::operator-(const LongInteger& rhs) const noexcept {
    LongInteger result = *this;
    result -= rhs;
    return result;
}

LongInteger& LongInteger::operator-=(const LongInteger& rhs) noexcept {
    if(sign == rhs.sign) {
        if (Abs() >= rhs.Abs()) {
            int carry = 0;
            for(int i = 0; i < (int)rhs.data.size() || carry; ++i) {
                const auto rhsValue = i < (int)rhs.data.size() ? rhs.data[i] : 0;
                data[i] -= carry + rhsValue;
                carry = 0;
                if(data[i] < 0) {
                    carry = 1;
                    data[i] += base;
                }
            }
            Trim();
            return *this;
        }
        LongInteger result = -rhs;
        return *this = result += *this;
    }
    return *this += (-rhs);
}

LongInteger LongInteger::operator*(const LongInteger& rhs) const noexcept {
    LongInteger result;
    for(size_t i = 0; i < rhs.data.size(); ++i) {
        auto summand = *this;
        summand.MultiplyByPowerOfBase(i);
        summand.MultiplyByLessThanBase(rhs.data[i]);
        result += summand;
    }
    result.sign = sign * rhs.sign;
    return result;
}

LongInteger& LongInteger::operator*=(const LongInteger& rhs) noexcept {
    return *this = *this * rhs;
}

LongInteger LongInteger::operator/(const LongInteger& rhs) const noexcept {
    LongInteger result, temp;
    DivideWithRemainder(rhs, result, temp);
    return result;
}

LongInteger& LongInteger::operator/=(const LongInteger& rhs) noexcept {
    return *this = *this / rhs;
}

LongInteger LongInteger::operator%(const LongInteger& rhs) const noexcept {
    LongInteger result, temp;
    DivideWithRemainder(rhs, temp, result);
    return result;
}

LongInteger& LongInteger::operator%=(const LongInteger& rhs) noexcept {
    return *this = *this % rhs;
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

void LongInteger::DivideByLessThanBase(const int64_t value) {
    if(value >= base) {
        throw std::runtime_error("Value must be less than base");
    }
    int64_t carry = 0;
    for(int i = data.size() - 1; i >= 0; --i) {
        int64_t block = (carry * base + data[i]) / value;
        carry = (carry * base + data[i]) % value;
        data[i] = block;
    }
    Trim();
}

void LongInteger::DivideWithRemainder(const LongInteger& divisor,
                         LongInteger& quotient,
                         LongInteger& remainder) const
{
    if(divisor.IsZero()) {
        throw std::runtime_error("Division by zero");
    }
    if(data.size() < divisor.data.size()) {
        quotient = 0;
        remainder = *this;
        return;
    }
    remainder = this->Abs();
    LongInteger subtrahend = divisor.Abs();
    subtrahend.MultiplyByPowerOfBase(data.size() - divisor.data.size());
    quotient = 0;
    LongInteger quotientSummand(1);
    quotientSummand.MultiplyByPowerOfBase(data.size() - divisor.data.size());
    while(subtrahend < remainder) {
        subtrahend.MultiplyByLessThanBase(10);
        quotientSummand.MultiplyByLessThanBase(10);
    }
    subtrahend.DivideByLessThanBase(10);
    quotientSummand.DivideByLessThanBase(10);

    while(remainder >= divisor && quotientSummand > 0) {
        while(remainder >= subtrahend) {
            remainder -= subtrahend;
            quotient += quotientSummand;
        }
        subtrahend.DivideByLessThanBase(10);
        quotientSummand.DivideByLessThanBase(10);
    }
    remainder.sign = sign;
    quotient.sign = sign * divisor.sign;
}

void LongInteger::MultiplyByLessThanBase(const int64_t value) {
    if(value > base) {
        throw std::runtime_error("Value must be less than base");
    }
    int64_t carry = 0;
    for(auto& block : data) {
        block = block * value + carry;
        carry = 0;
        if(block >= base) {
            carry = block / base;
            block %= base;
        }
    }
    if(carry > 0) {
        data.push_back(carry);
    }
}

void LongInteger::MultiplyByPowerOfBase(const uint64_t power) {
    for(uint64_t i = 0; i < power; ++i) {
        data.push_back(0);
    }
    for(int64_t i = data.size() - 1; i >= 0; --i) {
        if(i >= (int64_t)power) {
            data[i] = data[i - power];
        }
        else {
            data[i] = 0;
        }
    }
}

void LongInteger::Trim() noexcept {
    while(data.size() && !data.back()) {
        data.pop_back();
    }
}

std::ostream& operator<<(std::ostream& stream, const LongInteger& value) {
    return stream << value.ToString();
}

std::istream& operator>>(std::istream& stream, LongInteger& value) {
    std::string str;
    stream >> str;
    value = LongInteger(str);
    return stream;
}
