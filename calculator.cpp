#include "calculator.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>

bool is_valid_double(const std::string &expression) {
    // Empty string is not valid
    if (expression.empty()) {
        return false;
    }

    size_t start_idx = 0;
    // Check for sign at the beginning
    if (expression[0] == '+' || expression[0] == '-') {
        start_idx = 1;
    }

    // There must be at least one digit after the sign
    if (start_idx >= expression.length()) {
        return false;
    }

    bool decimal_point_found = false;
    bool digit_after_decimal = false;
    bool any_digit_found = false;

    for (size_t i = start_idx; i < expression.length(); i++) {
        char c = expression[i];

        if (c == '.') {
            // Decimal point can appear only once
            if (decimal_point_found) {
                return false;
            }
            decimal_point_found = true;
        } else if (c >= '0' && c <= '9') {
            any_digit_found = true;
            // If we found a digit after decimal point
            if (decimal_point_found) {
                digit_after_decimal = true;
            }
        } else {
            // Any other character makes it invalid
            return false;
        }
    }

    // If decimal point exists, there must be at least one digit after it
    if (decimal_point_found && !digit_after_decimal) {
        return false;
    }

    // Must have at least one digit
    return any_digit_found;
}

double parse_number(const std::string &expression) {
    // This function is implemented assuming expression is already validated
    // It's used for printing the result in a readable format

    bool negative = false;
    size_t start_idx = 0;

    if (expression[0] == '-') {
        negative = true;
        start_idx = 1;
    } else if (expression[0] == '+') {
        start_idx = 1;
    }

    double result = 0.0;
    double decimal_place = 1.0;
    bool decimal_found = false;

    for (size_t i = start_idx; i < expression.length(); i++) {
        if (expression[i] == '.') {
            decimal_found = true;
        } else {
            int digit = expression[i] - '0';
            if (!decimal_found) {
                result = result * 10 + digit;
            } else {
                decimal_place /= 10;
                result += digit * decimal_place;
            }
        }
    }

    return negative ? -result : result;
}

// Helper function to normalize a number by removing leading zeros
std::string normalize_number(const std::string &num) {
    if (num.empty()) return "0";

    // Split into sign, integer part, and decimal part
    bool negative = false;
    size_t start_idx = 0;

    if (num[0] == '-') {
        negative = true;
        start_idx = 1;
    } else if (num[0] == '+') {
        start_idx = 1;
    }

    // Find decimal point position
    size_t decimal_pos = num.find('.', start_idx);

    // Get integer part without leading zeros
    std::string integer_part;
    if (decimal_pos == std::string::npos) {
        integer_part = num.substr(start_idx);
    } else {
        integer_part = num.substr(start_idx, decimal_pos - start_idx);
    }

    // Remove leading zeros from integer part
    size_t first_non_zero = 0;
    while (first_non_zero < integer_part.length() && integer_part[first_non_zero] == '0') {
        first_non_zero++;
    }

    if (first_non_zero == integer_part.length()) {
        // All zeros in integer part
        integer_part = "0";
    } else {
        integer_part = integer_part.substr(first_non_zero);
    }

    // Handle decimal part if present
    std::string decimal_part = "";
    if (decimal_pos != std::string::npos) {
        decimal_part = num.substr(decimal_pos);

        // Remove trailing zeros from decimal part
        while (decimal_part.length() > 1 && decimal_part.back() == '0') {
            decimal_part.pop_back();
        }

        // If only '.' is left, remove it
        if (decimal_part == ".") {
            decimal_part = "";
        }
    }

    // Assemble the normalized number
    std::string result = (negative ? "-" : "") + integer_part + decimal_part;
    return result;
}

// Forward declare subtract_numbers to resolve the error
std::string subtract_numbers(const std::string &num1, const std::string &num2);

std::string add_numbers(const std::string &num1, const std::string &num2) {
    // Special cases for empty strings
    if (num1.empty()) return num2;
    if (num2.empty()) return num1;

    // Determine sign of each number
    bool num1_negative = (!num1.empty() && num1[0] == '-');
    bool num2_negative = (!num2.empty() && num2[0] == '-');

    // Handle different sign combinations by turning into appropriate addition or subtraction
    if (num1_negative && !num2_negative) {
        // -a + b = b - a
        std::string abs_num1 = num1.substr(1);
        return subtract_numbers(num2, abs_num1);
    } else if (!num1_negative && num2_negative) {
        // a + (-b) = a - b
        std::string abs_num2 = num2.substr(1);
        return subtract_numbers(num1, abs_num2);
    }

    // Now both numbers have the same sign
    bool result_negative = num1_negative; // Result sign will match both inputs

    // Remove sign for calculation
    std::string abs_num1 = num1_negative ? num1.substr(1) : num1;
    std::string abs_num2 = num2_negative ? num2.substr(1) : num2;

    // Remove leading '+' if present
    if (!abs_num1.empty() && abs_num1[0] == '+') abs_num1 = abs_num1.substr(1);
    if (!abs_num2.empty() && abs_num2[0] == '+') abs_num2 = abs_num2.substr(1);

    // Split numbers into integer and decimal parts
    size_t decimal_pos1 = abs_num1.find('.');
    size_t decimal_pos2 = abs_num2.find('.');

    std::string int_part1, decimal_part1, int_part2, decimal_part2;

    if (decimal_pos1 != std::string::npos) {
        int_part1 = abs_num1.substr(0, decimal_pos1);
        decimal_part1 = abs_num1.substr(decimal_pos1 + 1);
    } else {
        int_part1 = abs_num1;
        decimal_part1 = "";
    }

    if (decimal_pos2 != std::string::npos) {
        int_part2 = abs_num2.substr(0, decimal_pos2);
        decimal_part2 = abs_num2.substr(decimal_pos2 + 1);
    } else {
        int_part2 = abs_num2;
        decimal_part2 = "";
    }

    // Pad decimal parts to the same length
    size_t max_decimal_length = std::max(decimal_part1.length(), decimal_part2.length());
    decimal_part1.append(max_decimal_length - decimal_part1.length(), '0');
    decimal_part2.append(max_decimal_length - decimal_part2.length(), '0');

    // Add decimal parts
    std::string result_decimal = "";
    int carry = 0;

    for (int i = max_decimal_length - 1; i >= 0; i--) {
        int sum = (decimal_part1[i] - '0') + (decimal_part2[i] - '0') + carry;
        carry = sum / 10;
        result_decimal = std::to_string(sum % 10) + result_decimal;
    }

    // Add integer parts
    std::string result_int = "";

    // Pad integer parts to the same length for easier addition
    while (int_part1.length() < int_part2.length()) {
        int_part1 = "0" + int_part1;
    }
    while (int_part2.length() < int_part1.length()) {
        int_part2 = "0" + int_part2;
    }

    for (int i = int_part1.length() - 1; i >= 0; i--) {
        int sum = (int_part1[i] - '0') + (int_part2[i] - '0') + carry;
        carry = sum / 10;
        result_int = std::to_string(sum % 10) + result_int;
    }

    // Handle final carry
    if (carry > 0) {
        result_int = std::to_string(carry) + result_int;
    }

    // Combine results
    std::string result;
    if (result_decimal.empty()) {
        result = result_int;
    } else {
        result = result_int + "." + result_decimal;
    }

    // Add sign and normalize
    if (result_negative && result != "0") {
        result = "-" + result;
    }

    return normalize_number(result);
}

// Helper function for subtraction (needed for addition with different signs)
std::string subtract_numbers(const std::string &num1, const std::string &num2) {
    // Special cases for empty strings
    if (num1.empty()) return "-" + num2;
    if (num2.empty()) return num1;

    // Handle leading '+' signs
    std::string processed_num1 = num1;
    std::string processed_num2 = num2;

    if (!processed_num1.empty() && processed_num1[0] == '+') processed_num1 = processed_num1.substr(1);
    if (!processed_num2.empty() && processed_num2[0] == '+') processed_num2 = processed_num2.substr(1);

    // If both numbers are equal, result is 0
    if (processed_num1 == processed_num2) return "0";

    // Split numbers into integer and decimal parts
    size_t decimal_pos1 = processed_num1.find('.');
    size_t decimal_pos2 = processed_num2.find('.');

    std::string int_part1, decimal_part1, int_part2, decimal_part2;

    if (decimal_pos1 != std::string::npos) {
        int_part1 = processed_num1.substr(0, decimal_pos1);
        decimal_part1 = processed_num1.substr(decimal_pos1 + 1);
    } else {
        int_part1 = processed_num1;
        decimal_part1 = "";
    }

    if (decimal_pos2 != std::string::npos) {
        int_part2 = processed_num2.substr(0, decimal_pos2);
        decimal_part2 = processed_num2.substr(decimal_pos2 + 1);
    } else {
        int_part2 = processed_num2;
        decimal_part2 = "";
    }

    // Pad decimal parts to the same length
    size_t max_decimal_length = std::max(decimal_part1.length(), decimal_part2.length());
    decimal_part1.append(max_decimal_length - decimal_part1.length(), '0');
    decimal_part2.append(max_decimal_length - decimal_part2.length(), '0');

    // Remove leading zeros for comparison
    std::string clean_int_part1 = int_part1;
    std::string clean_int_part2 = int_part2;

    while (!clean_int_part1.empty() && clean_int_part1[0] == '0') clean_int_part1.erase(0, 1);
    while (!clean_int_part2.empty() && clean_int_part2[0] == '0') clean_int_part2.erase(0, 1);

    if (clean_int_part1.empty()) clean_int_part1 = "0";
    if (clean_int_part2.empty()) clean_int_part2 = "0";

    // Determine which number is larger to ensure we subtract smaller from larger
    bool num1_larger = true;

    if (clean_int_part1.length() < clean_int_part2.length()) {
        num1_larger = false;
    } else if (clean_int_part1.length() == clean_int_part2.length()) {
        if (clean_int_part1 < clean_int_part2) {
            num1_larger = false;
        } else if (clean_int_part1 == clean_int_part2) {
            if (decimal_part1 < decimal_part2) {
                num1_larger = false;
            }
        }
    }

    // Swap numbers if num2 is larger
    std::string larger_int, larger_decimal, smaller_int, smaller_decimal;
    bool result_negative = false;

    if (num1_larger) {
        larger_int = int_part1;
        larger_decimal = decimal_part1;
        smaller_int = int_part2;
        smaller_decimal = decimal_part2;
    } else {
        larger_int = int_part2;
        larger_decimal = decimal_part2;
        smaller_int = int_part1;
        smaller_decimal = decimal_part1;
        result_negative = true;
    }

    // Pad integer parts for easier subtraction
    while (smaller_int.length() < larger_int.length()) {
        smaller_int = "0" + smaller_int;
    }

    // Subtract decimal parts
    std::string result_decimal = "";
    int borrow = 0;

    for (int i = max_decimal_length - 1; i >= 0; i--) {
        int digit1 = larger_decimal[i] - '0';
        int digit2 = smaller_decimal[i] - '0' + borrow;
        borrow = 0;

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }

        result_decimal = std::to_string(digit1 - digit2) + result_decimal;
    }

    // Subtract integer parts
    std::string result_int = "";

    for (int i = larger_int.length() - 1; i >= 0; i--) {
        int digit1 = larger_int[i] - '0' - borrow;
        int digit2 = smaller_int[i] - '0';
        borrow = 0;

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }

        result_int = std::to_string(digit1 - digit2) + result_int;
    }

    // Combine results
    std::string result;
    if (result_decimal.empty()) {
        result = result_int;
    } else {
        result = result_int + "." + result_decimal;
    }

    // Add sign if needed and normalize
    if (result_negative && result != "0") {
        result = "-" + result;
    }

    return normalize_number(result);
}

std::string multiply_numbers(const std::string &num1, const std::string &num2) {
    // Special cases for empty strings
    if (num1.empty() || num2.empty()) return "0";

    // Determine sign of result
    bool num1_sign = !num1.empty() && num1[0] == '-';
    bool num2_sign = !num2.empty() && num2[0] == '-';
    bool result_negative = num1_sign != num2_sign;

    // Remove signs for calculation
    std::string abs_num1 = (num1_sign || (!num1.empty() && num1[0] == '+')) ? num1.substr(1) : num1;
    std::string abs_num2 = (num2_sign || (!num2.empty() && num2[0] == '+')) ? num2.substr(1) : num2;

    // Count decimal places
    int decimal_places1 = 0, decimal_places2 = 0;
    size_t decimal_pos1 = abs_num1.find('.');
    size_t decimal_pos2 = abs_num2.find('.');

    if (decimal_pos1 != std::string::npos) {
        decimal_places1 = abs_num1.length() - decimal_pos1 - 1;
        abs_num1.erase(decimal_pos1, 1); // Remove decimal point
    }

    if (decimal_pos2 != std::string::npos) {
        decimal_places2 = abs_num2.length() - decimal_pos2 - 1;
        abs_num2.erase(decimal_pos2, 1); // Remove decimal point
    }

    // Remove leading zeros
    while (!abs_num1.empty() && abs_num1[0] == '0') {
        abs_num1.erase(0, 1);
    }
    while (!abs_num2.empty() && abs_num2[0] == '0') {
        abs_num2.erase(0, 1);
    }

    // If any number is zero, result is zero
    if (abs_num1.empty() || abs_num2.empty()) {
        return "0";
    }

    // Perform multiplication
    std::vector<int> result(abs_num1.length() + abs_num2.length(), 0);

    for (int i = abs_num1.length() - 1; i >= 0; i--) {
        for (int j = abs_num2.length() - 1; j >= 0; j--) {
            int digit1 = abs_num1[i] - '0';
            int digit2 = abs_num2[j] - '0';

            int pos1 = i + j + 1;
            int pos2 = i + j;

            int mul = digit1 * digit2 + result[pos1];

            result[pos1] = mul % 10;
            result[pos2] += mul / 10;
        }
    }

    // Convert result to string
    std::string result_str = "";
    for (int val : result) {
        result_str += std::to_string(val);
    }

    // Remove leading zeros
    while (!result_str.empty() && result_str[0] == '0') {
        result_str.erase(0, 1);
    }

    // If result is empty, it was all zeros
    if (result_str.empty()) {
        return "0";
    }

    // Total decimal places in the result
    int total_decimal_places = decimal_places1 + decimal_places2;

    // Insert decimal point if needed
    if (total_decimal_places > 0) {
        if (result_str.length() <= static_cast<size_t>(total_decimal_places)) {
            result_str = std::string(total_decimal_places - result_str.length() + 1, '0') + result_str;
        }
        result_str.insert(result_str.length() - total_decimal_places, ".");
    }

    // Add sign if needed
    if (result_negative && result_str != "0") {
        result_str = "-" + result_str;
    }

    return normalize_number(result_str);
}

std::vector<std::string> load_numbers_from_file(const std::string &filename) {
    std::vector<std::string> numbers;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return numbers;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove any trailing whitespace
        size_t end = line.find_last_not_of(" \n\r\t");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        } else if (line.empty()) {
            continue; // Skip empty lines
        }
        numbers.push_back(line);
    }

    file.close();
    return numbers;
}

