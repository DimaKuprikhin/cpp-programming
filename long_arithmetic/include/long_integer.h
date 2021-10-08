#pragma once
#include <string>
#include <type_traits>
#include <vector>

class LongInteger {
public:
    LongInteger() noexcept;

    LongInteger(int64_t value) noexcept;

    LongInteger(const std::string& number);

    LongInteger(const LongInteger& other) noexcept;

    LongInteger& operator=(const LongInteger& other) noexcept;

    LongInteger operator+() const noexcept;

    LongInteger operator-() const noexcept;

    LongInteger operator+(const LongInteger& rhs) const noexcept;

    LongInteger& operator+=(const LongInteger& rhs) noexcept;

    LongInteger operator-(const LongInteger& rhs) const noexcept;

    LongInteger& operator-=(const LongInteger& rhs) noexcept;

    LongInteger operator*(const LongInteger& rhs) const noexcept;

    LongInteger& operator*=(const LongInteger& rhs) noexcept;

    LongInteger operator/(const LongInteger& rhs) const noexcept;

    LongInteger& operator/=(const LongInteger& rhs) noexcept;

    LongInteger operator%(const LongInteger& rhs) const noexcept;

    LongInteger& operator%=(const LongInteger& rhs) noexcept;

    bool operator<(const LongInteger& rhs) const noexcept;

    bool operator>(const LongInteger& rhs) const noexcept;

    bool operator<=(const LongInteger& rhs) const noexcept;

    bool operator>=(const LongInteger& rhs) const noexcept;

    bool operator==(const LongInteger& rhs) const noexcept;

    LongInteger Abs() const noexcept;

    bool IsZero() const noexcept;

    std::string ToString() const noexcept;

private:
    void DivideByLessThanBase(const int64_t value);

    void DivideByPowerOfBase(const uint64_t power);

    void DivideWithRemainder(const LongInteger& divisor,
                             LongInteger& quotient,
                             LongInteger& remainder) const;

    void MultiplyByLessThanBase(const int64_t value);

    void MultiplyByPowerOfBase(const uint64_t power);

    void Trim() noexcept;

private:
    static const int64_t base = 1e9;
    int32_t sign;
    std::vector<int64_t> data;
};

std::ostream& operator<<(std::ostream& stream, const LongInteger& value);

std::istream& operator>>(std::istream& stream, LongInteger& value);
