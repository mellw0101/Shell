#ifndef MATH_H
#define MATH_H

#include <string>
#include <iostream>
#include <vector>

namespace tools {
    double add(double a, double b);                     // Function to add two numbers
    double subtract(double a, double b);                     // Function to subtract two numbers
    double multiply(double a, double b);               // Function to multiply two numbers
    double divide(double a, double b);                 // Function to divide two numbers (checks for division by zero)
    double solveLinearEquation(double a, double b);
}

void math(const std::vector<std::string>& args);

#endif // MATH_H
