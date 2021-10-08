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

LongInteger LongInteger::operator+() const {
    return *this;
}

LongInteger LongInteger::operator-() const {
    auto result(*this);
    result.sign *= -1;
    return result;
}

LongInteger LongInteger::operator+(const LongInteger& rhs) const {
    auto result = *this;
    result += rhs;
    return result;
}

LongInteger& LongInteger::operator+=(const LongInteger& rhs) {
    if(sign == rhs.sign) {
        ElementWiseAddition(*this, rhs, *this);
    }
    else {
        if(LessAbsoluteValue(rhs, *this)) {
            ElementWiseSubtraction(*this, rhs, *this);
        }
        else {
            ElementWiseSubtraction(rhs, *this, *this);
            sign = -sign;
        }
    }
    return *this;
}

LongInteger LongInteger::operator-(const LongInteger& rhs) const {
    LongInteger result = *this;
    result -= rhs;
    return result;
}

LongInteger& LongInteger::operator-=(const LongInteger& rhs) {
    if(sign != rhs.sign) {
        ElementWiseAddition(*this, rhs, *this);
    }
    else {
        if(LessAbsoluteValue(rhs, *this)) {
            ElementWiseSubtraction(*this, rhs, *this);
        }
        else {
            ElementWiseSubtraction(rhs, *this, *this);
            sign = -sign;
        }
    }
    return *this;
}

LongInteger LongInteger::operator*(const LongInteger& rhs) const {
    auto result = *this;
    result *= rhs;
    return result;
}

LongInteger& LongInteger::operator*=(const LongInteger& rhs) {
    LongInteger result;
    for(size_t i = 0; i < rhs.data.size(); ++i) {
        auto summand = *this;
        summand.MultiplyByPowerOfBase(i);
        summand.MultiplyByLessThanBase(rhs.data[i]);
        result += summand;
    }
    result.sign = sign * rhs.sign;
    return *this = std::move(result);
}

LongInteger LongInteger::operator/(const LongInteger& rhs) const {
    auto result = *this;
    result /= rhs;
    return result;
}

LongInteger& LongInteger::operator/=(const LongInteger& rhs) {
    LongInteger result, temp;
    DivideWithRemainder(rhs, result, temp);
    return *this = std::move(result);
}

LongInteger LongInteger::operator%(const LongInteger& rhs) const {
    auto result = *this;
    result %= rhs;
    return result;
}

LongInteger& LongInteger::operator%=(const LongInteger& rhs) {
    LongInteger result, temp;
    DivideWithRemainder(rhs, temp, result);
    return *this = std::move(result);
}

bool LongInteger::operator<(const LongInteger& rhs) const noexcept {
    if(IsZero() && rhs.IsZero()) {
        return false;
    }
    if(sign != rhs.sign) {
        return sign == -1;
    }
    return (sign == 1 && LessAbsoluteValue(*this, rhs))
            || (sign == -1 && LessAbsoluteValue(rhs, *this));
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

LongInteger LongInteger::Abs() const {
    auto result(*this);
    result.sign *= result.sign;
    return result;
}

bool LongInteger::IsZero() const noexcept {
    return !data.size();
}

std::string LongInteger::ToString() const {
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

int64_t LongInteger::At(size_t index) const noexcept {
    if(index < data.size()) {
        return data[index];
    }
    return 0;
}

void LongInteger::ElementWiseAddition(
    const LongInteger& first,
    const LongInteger& second,
    LongInteger& result) 
{
    result.data.resize(std::max(first.data.size(), second.data.size()) + 1, 0);
    int carry = 0;
    for(int i = 0; i < static_cast<int>(result.data.size()); ++i) {
        result.data[i] = carry + first.At(i) + second.At(i);
        carry = 0;
        if(result.data[i] >= base) {
            result.data[i] -= base;
            carry = 1;
        }
    }
    result.Trim();
}

void LongInteger::ElementWiseSubtraction(
    const LongInteger& lhs,
    const LongInteger& rhs,
    LongInteger& result)
{
    result.data.resize(std::max(lhs.data.size(), rhs.data.size()), 0);
    int carry = 0;
    for(int i = 0; i < static_cast<int>(result.data.size()); ++i) {
        result.data[i] = lhs.At(i) - rhs.At(i) - carry;
        carry = 0;
        if(result.data[i] < 0) {
            result.data[i] += base;
            carry = 1;
        }
    }
    result.Trim();
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

bool LongInteger::LessAbsoluteValue(
    const LongInteger& lhs,
    const LongInteger& rhs) noexcept
{
    if(lhs.data.size() != rhs.data.size()) {
        return lhs.data.size() < rhs.data.size();
    }
    for(int i = lhs.data.size() - 1; i >= 0; --i) {
        if(lhs.At(i) != rhs.At(i)) {
            return lhs.At(i) < rhs.At(i);
        }
    }
    return false;
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
    for(int i = 0; i < static_cast<int>(power); ++i) {
        data.push_back(0);
    }
    for(int i = data.size() - 1; i >= 0; --i) {
        if(i >= static_cast<int>(power)) {
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

LongInteger operator"" _li(unsigned long long int value) {
    return LongInteger(value);
}

LongInteger operator"" _li(const char* string, size_t size) {
    LongInteger result(std::string(string, size));
    return result;
}
