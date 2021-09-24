#pragma once
#include <string>
#include <vector>

class LongInteger {
public:
    LongInteger() noexcept;

    LongInteger(int64_t value) noexcept;

    LongInteger(const std::string& number);

    LongInteger operator+() const noexcept;

    LongInteger operator-() const noexcept;

    LongInteger operator+(const LongInteger& rhs) const noexcept;

    LongInteger operator-(const LongInteger& rhs) const noexcept;

    bool operator<(const LongInteger& rhs) const noexcept;

    bool operator>(const LongInteger& rhs) const noexcept;

    bool operator<=(const LongInteger& rhs) const noexcept;

    bool operator>=(const LongInteger& rhs) const noexcept;

    bool operator==(const LongInteger& rhs) const noexcept;

    LongInteger Abs() const noexcept;

    bool IsZero() const noexcept;

    std::string ToString() const noexcept;

private:
    void Trim() noexcept;

private:
    const int64_t base = 1e9;
    int32_t sign;
    std::vector<int64_t> data;
};
