#include <iostream>
#include <string>
#include "calculator.hpp"

int main(int argc, char *argv[]) {
    std::string filename;

    // Get filename from command line argument or prompt user
    if (argc > 1) {
        filename = argv[1];
    } else {
        std::cout << "Enter the input filename: ";
        std::cin >> filename;
    }

    // Load numbers from file
    std::vector<std::string> numbers = load_numbers_from_file(filename);

    // The constant for addition
    std::string constant = "-123.456";

    // Process each number
    for (const std::string &number : numbers) {
        std::cout << "Processing: \"" << number << "\"" << std::endl;

        // Check if the number is valid
        if (is_valid_double(number)) {
            std::cout << "  Valid double number" << std::endl;

            // Print the parsed value for verification
            double parsed = parse_number(number);
            std::cout << "  Parsed value: " << parsed << std::endl;

            // Perform addition with -123.456
            std::string sum = add_numbers(number, constant);

            // Output result
            std::cout << "  " << number << " + " << constant << " = " << sum << std::endl;
            double expected_result = parsed + (-123.456); // for verification
            std::cout << "  Expected result: " << expected_result << std::endl;
        } else {
            std::cout << "  Invalid double number" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}

