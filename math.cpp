#include "math.h"


namespace tools {
    double add(double a, double b) {                    // Function to add two numbers
        return a + b;
    }
    double subtract(double a, double b) {               // Function to subtract two numbers
        return a - b;
    }
    double multiply(double a, double b) {               // Function to multiply two numbers
        return a * b;
    }
    double divide(double a, double b) {                 // Function to divide two numbers (checks for division by zero)
        if (b == 0) {
            std::cerr << "Error: You have violated the law, math will not accsetpt devision by '0' STOP NOW!!" << std::endl;
            return 0; // You can handle this error differently if needed
        }
        return a / b;
    }
    double solveLinearEquation(double a, double b) {    // Function to solve linear equations ax + b = 0
        if (a == 0) {
            if (b == 0) {
                std::cout << "Infinite solutions (any x will work)." << std::endl;
            } else {
                std::cout << "No solution (contradiction)." << std::endl;
            }
            return 0; // Not-a-Number to indicate no valid solution
        } else {
            double x = -b / a;
            std::cout << "Solution: x = " << x << std::endl;
            return x;
        }
    }
}
void math(const std::vector<std::string>& args) {
    if (args.size() == 4) {
        double result1 = strtod(args[2].c_str(), nullptr);
        double result2 = strtod(args[3].c_str(), nullptr);
        if (args[1] == "add") {
            double result = tools::add(result1, result2);
            std::cout << "math: '" << result1 << "' + '" << result2 << "' = '" << result << "'\n";
        } else if (args[1] == "sub") {
            double result = tools::subtract(result1, result2);
            std::cout << "math: '" << result1 << "' - '" << result2 << "' = '" << result << "'\n";
        } else if (args[1] == "div") {
            double result = tools::divide(result1, result2);
            std::cout << "math: '" << result1 << "' / '" << result2 << "' = '" << result << "'\n";
        } else if (args[1] == "multi") {
            double result = tools::multiply(result1, result2);
            std::cout << "math: '" << result1 << "' * '" << result2 << "' = '" << result << "'\n";
        } else {
            std::cout << "usage: math <add, sub, div, multi> <num1> <num2>\n";
        }
    } else {
        std::cout << "usage: math <add, sub, div, multi> <num1> <num2>\n";
    }
}
