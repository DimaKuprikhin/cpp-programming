#pragma once
#include <vector>
#include <long_integer.h>

const int testIntegers = 6;

const std::vector<LongInteger> integers({
    0_li, 1_li, 1000000000_li,
    "1000000000000000000"_li,
    "-1"_li, "-1000000000"_li
});

const std::vector<std::vector<LongInteger>> plusResults({
    // 0_li + integers[i]
    {
        0_li, 1_li, 1000000000_li,
        "1000000000000000000"_li,
        "-1"_li, "-1000000000"_li
    },
    // 1_li + integers[i]
    {
        1_li, 2_li, 1000000001_li,
        "1000000000000000001"_li,
        0_li, "-999999999"_li
    },
    // 1000000000_li + integers[i]
    {
        1000000000_li, 1000000001_li, 2000000000_li,
        "1000000001000000000"_li,
        999999999_li, 0_li
    },
    // "1000000000000000000"_li + integers[i]
    {
        "1000000000000000000"_li, "1000000000000000001"_li, "1000000001000000000"_li,
        "2000000000000000000"_li,
        "999999999999999999"_li, "999999999000000000"_li
    },
    // "-1"_li + integers[i]
    {
        "-1"_li, 0_li, 999999999_li,
        "999999999999999999"_li,
        "-2"_li, "-1000000001"_li
    },
    // "-1000000000"_li + integers[i]
    {
        "-1000000000"_li, "-999999999"_li, 0_li,
        "999999999000000000"_li,
        "-1000000001"_li, "-2000000000"_li
    }
});

const std::vector<std::vector<LongInteger>> minusResults({
    // 0_li - integers[i]
    {
        0_li, "-1"_li, "-1000000000"_li,
        "-1000000000000000000"_li,
        1_li, 1000000000_li
    },
    // 1_li - integers[i]
    {
        1_li, 0_li, "-999999999"_li,
        "-999999999999999999"_li,
        2_li, 1000000001_li
    },
    // 1000000000_li - integers[i]
    {
        1000000000_li, 999999999_li, 0_li,
        "-999999999000000000"_li,
        1000000001_li, 2000000000_li
    },
    // "1000000000000000000"_li - integers[i]
    {
        "1000000000000000000"_li, "999999999999999999"_li, "999999999000000000"_li,
        0_li,
        "1000000000000000001"_li, "1000000001000000000"_li
    },
    // "-1"_li - integers[i]
    {
        "-1"_li, "-2"_li, "-1000000001"_li,
        "-1000000000000000001"_li,
        0_li, 999999999_li
    },
    // "-1000000000"_li - integers[i]
    {
        "-1000000000"_li, "-1000000001"_li, "-2000000000"_li,
        "-1000000001000000000"_li,
        "-999999999"_li, 0_li
    }
});
