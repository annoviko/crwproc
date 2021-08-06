#include "intro_builder.h"


void intro_builder::show(const context& p_context, const std::string& p_intro) {
    static const std::string line(60, '-');

    std::cout << line << std::endl;
    std::cout << p_context;
    std::cout << line << std::endl;

    std::cout << line << std::endl;
    std::cout << p_intro << std::endl;
    std::cout << line << std::endl;
}