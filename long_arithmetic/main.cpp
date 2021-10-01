#include "long_integer.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <map>
#include <functional>

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cout << "Using: ./long_arithmetic [FIRST_NUMBER] [OPERATION] [SECOND_NUMBER]"
                  << std::endl;
        return 0;
    }

    std::string operation;
    LongInteger lhs, rhs;
    try {
        lhs = std::string(argv[1]);
        operation = argv[2];
        rhs = std::string(argv[3]);
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return 0;
    }

    std::map<std::string, std::function<LongInteger(const LongInteger&, const LongInteger&)>>
    integerOperations({
        { "+", [](const auto& lhs, const auto& rhs)->auto { return lhs + rhs; } },
        { "-", [](const auto& lhs, const auto& rhs)->auto { return lhs - rhs; } },
        { "*", [](const auto& lhs, const auto& rhs)->auto { return lhs * rhs; } },
        { "/", [](const auto& lhs, const auto& rhs)->auto { return lhs / rhs; } },
        { "%", [](const auto& lhs, const auto& rhs)->auto { return lhs % rhs; } }
    });
    std::map<std::string, std::function<bool(const LongInteger&, const LongInteger&)>>
    logicalOperations({
        { "<", [](const auto& lhs, const auto& rhs)->auto { return lhs < rhs; } },
        { ">", [](const auto& lhs, const auto& rhs)->auto { return lhs > rhs; } },
        { "<=", [](const auto& lhs, const auto& rhs)->auto { return lhs <= rhs; } },
        { ">=", [](const auto& lhs, const auto& rhs)->auto { return lhs >= rhs; } },
        { "==", [](const auto& lhs, const auto& rhs)->auto { return lhs == rhs; } }
    });

    if(integerOperations.find(operation) != integerOperations.end()) {
        std::cout << integerOperations.find(operation)->second(lhs, rhs).ToString()
                  << std::endl;
    }
    else if(logicalOperations.find(operation) != logicalOperations.end()) {
        std::cout << (logicalOperations.find(operation)->second(lhs, rhs) ? "True" : "False")
                  << std::endl;
    }
    else {
        std::cout << "Wrong operation. Possible operations: +, -, *, /, %, <, >, <=, >=, ==" << std::endl;
    }
}
