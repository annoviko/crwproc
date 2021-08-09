#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>

#include <windows.h>
#include <psapi.h>


struct context {
    std::int8_t m_int8 = 64;
    std::int16_t m_int16 = 1024;
    std::int32_t m_int32 = 1234567;
    std::int64_t m_int64 = 1234567890;

    float m_float = 3.14;
    double m_double = 1.2345;
};


void show_values(const std::size_t p_pid, const context& g_context) {
    std::cout << p_pid << ") " << (int)g_context.m_int8 << ", " << g_context.m_int16 << ", " << g_context.m_int32 << ", " << g_context.m_int64 << ", " << g_context.m_float << ", " << g_context.m_double << std::endl;
}


int main() {
    DWORD pid = GetCurrentProcessId();

    context g_context;

    show_values(pid, g_context);

    while (true) {

        std::cout << "Enter command: ";

        std::string command;
        std::cin >> command;

        if (command == "inc") {
            g_context.m_int8++;
            g_context.m_int16++;
            g_context.m_int32++;
            g_context.m_int64++;

            g_context.m_float += 0.1;
            g_context.m_double += 0.1;
        }
        else if (command == "dec") {
            g_context.m_int8--;
            g_context.m_int16--;
            g_context.m_int32--;
            g_context.m_int64--;

            g_context.m_float -= 0.1;
            g_context.m_double -= 0.1;
        }
        else if (command == "addr") {
            std::cout << std::setw(10) << (int) g_context.m_int8 << ": " << (void*)&g_context.m_int8 << std::endl;
            std::cout << std::setw(10) << g_context.m_int16 << ": " << (void*)&g_context.m_int16 << std::endl;
            std::cout << std::setw(10) << g_context.m_int32 << ": " << (void*)&g_context.m_int32 << std::endl;
            std::cout << std::setw(10) << g_context.m_int64 << ": " << (void*)&g_context.m_int64 << std::endl;

            std::cout << std::setw(10) << g_context.m_float << ": " << (void*)&g_context.m_float << std::endl;
            std::cout << std::setw(10) << g_context.m_double << ": " << (void*)&g_context.m_double << std::endl;
        }
        else if (command == "show") {
            show_values(pid, g_context);
        }
        else {
            std::cout << "ERROR: WRONG COMMAND." << std::endl;
        }
    }

    return 0;
}