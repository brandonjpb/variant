#include <iostream>
#include <string>
#include <variant>
#include <typeinfo>

using VariantType = std::variant<int, double, std::string>;

void process_variant(const VariantType& var) {
    // Print value using std::visit
    std::cout << "Value: ";
    std::visit([](const auto& val) {
        std::cout << val;
        }, var);
    std::cout << "\n";

    // Print actual type
    std::cout << "Type: ";
    std::visit([](const auto& val) {
        using T = std::decay_t<decltype(val)>; // Decay strips val of any references; turns it into it's "vanilla" counterpart (easier to work with)
        if constexpr (std::is_same_v<T, int>) // Returns true if <This, and That> are the same data type; false if not
            std::cout << "int";
        else if constexpr (std::is_same_v<T, double>)
            std::cout << "double";
        else if constexpr (std::is_same_v<T, std::string>)
            std::cout << "string";
        }, var);
    std::cout << "\n";

    // Try to get the wrong type using std::get (throws exception)
    std::cout << "Exception demo: ";
    try {
        // Always try to access a type that's not what we have
        if (std::holds_alternative<int>(var)) {
            std::cout << std::get<double>(var);
        }
        else {
            std::cout << std::get<int>(var);
        }
    }
    catch (const std::bad_variant_access& e) {
        std::cout << "Caught exception: " << e.what();
    }
    std::cout << "\n";

    // Use get_if to safely access without exception
    std::cout << "Using get_if: ";
    if (const int* i = std::get_if<int>(&var)) {
        std::cout << "It's an int with value " << *i;
    }
    else if (const double* d = std::get_if<double>(&var)) {
        std::cout << "It's a double with value " << *d;
    }
    else if (const std::string* s = std::get_if<std::string>(&var)) {
        std::cout << "It's a string with value \"" << *s << '\n';
    }
    std::cout << "\n";

    std::cout << "-----------------\n";
}

int main() {
    // Create examples of each type
    VariantType int_var = 42;
    VariantType double_var = 3.14159;
    VariantType string_var = std::string("Hello, variant!");

    // Process each variant
    process_variant(int_var);
    process_variant(double_var);
    process_variant(string_var);

    return 0;
}