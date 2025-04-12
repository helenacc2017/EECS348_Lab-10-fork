#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>
#include <vector>

// Check if a string represents a valid double number
bool is_valid_double(const std::string &expression);

// Parse a valid string to a double (assumes expression is already validated)
double parse_number(const std::string &expression);

// Add two string-formatted double numbers without converting to double type
std::string add_numbers(const std::string &num1, const std::string &num2);

// Multiply two string-formatted double numbers without converting to double type
std::string multiply_numbers(const std::string &num1, const std::string &num2);

// Load numbers from a file
std::vector<std::string> load_numbers_from_file(const std::string &filename);

#endif // __CALCULATOR_HPP