#include <iostream>
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
    double m_double = 1.23456789;
};


context g_context;


void show_values(const std::size_t p_pid) {
    std::cout << p_pid << ") " << (int)g_context.m_int8 << ", " << g_context.m_int16 << ", " << g_context.m_int32 << ", " << g_context.m_int64 << ", " << g_context.m_float << ", " << g_context.m_double << std::endl;
}


int main() {
    DWORD pid = GetCurrentProcessId();

    show_values(pid);

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
        else if (command == "show") {
            show_values(pid);
        }
        else {
            std::cout << "ERROR: WRONG COMMAND." << std::endl;
        }
    }

    return 0;
}