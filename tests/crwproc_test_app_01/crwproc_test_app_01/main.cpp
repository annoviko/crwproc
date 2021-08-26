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

    std::int8_t* m_ptr_int8 = new std::int8_t(16);
    std::int16_t* m_ptr_int16 = new std::int16_t(512);
    std::int32_t* m_ptr_int32 = new std::int32_t(100000);
    std::int64_t* m_ptr_int64 = new std::int64_t(1000000000);
};


std::int8_t g_int8 = 32;
std::int16_t g_int16 = 512;
std::int32_t g_int32 = 77512;
std::int64_t g_int64 = 3333333333;


void show_values(const std::size_t p_pid, const context& context) {
    std::cout << p_pid << ") [data]  " << (int)g_int8 << ", " << g_int16 << ", " << g_int32 << ", " << g_int64 << std::endl;
    std::cout << p_pid << ") [stack] " << (int) context.m_int8 << ", " << context.m_int16 << ", " << context.m_int32 << ", " << context.m_int64 << ", " << context.m_float << ", " << context.m_double << std::endl;
    std::cout << p_pid << ") [ptrs]  " << (int)*context.m_ptr_int8 << ", " << *context.m_ptr_int16 << ", " << *context.m_ptr_int32 << ", " << *context.m_ptr_int64 << std::endl;
}


int main() {
    DWORD pid = GetCurrentProcessId();

    context context;

    show_values(pid, context);

    while (true) {

        std::cout << "Enter command: ";

        std::string command;
        std::cin >> command;

        if (command == "inc") {
            context.m_int8++;
            context.m_int16++;
            context.m_int32++;
            context.m_int64++;

            context.m_float += 0.1;
            context.m_double += 0.1;

            (*context.m_ptr_int8)++;
            (*context.m_ptr_int16)++;
            (*context.m_ptr_int32)++;
            (*context.m_ptr_int64)++;

            g_int8++;
            g_int16++;
            g_int32++;
            g_int64++;
        }
        else if (command == "dec") {
            context.m_int8--;
            context.m_int16--;
            context.m_int32--;
            context.m_int64--;

            context.m_float -= 0.1;
            context.m_double -= 0.1;

            (*context.m_ptr_int8)--;
            (*context.m_ptr_int16)--;
            (*context.m_ptr_int32)--;
            (*context.m_ptr_int64)--;

            g_int8--;
            g_int16--;
            g_int32--;
            g_int64--;
        }
        else if (command == "addr") {
            std::cout << "[data]" << std::endl;
            std::cout << std::setw(10) << (int)g_int8 << ": " << (void*)&g_int8 << std::endl;
            std::cout << std::setw(10) << g_int16 << ": " << (void*)&g_int16 << std::endl;
            std::cout << std::setw(10) << g_int32 << ": " << (void*)&g_int32 << std::endl;
            std::cout << std::setw(10) << g_int64 << ": " << (void*)&g_int64 << std::endl;

            std::cout << std::endl << "[stack]" << std::endl;
            std::cout << std::setw(10) << (int) context.m_int8 << ": " << (void*)&context.m_int8 << std::endl;
            std::cout << std::setw(10) << context.m_int16 << ": " << (void*)&context.m_int16 << std::endl;
            std::cout << std::setw(10) << context.m_int32 << ": " << (void*)&context.m_int32 << std::endl;
            std::cout << std::setw(10) << context.m_int64 << ": " << (void*)&context.m_int64 << std::endl;

            std::cout << std::setw(10) << context.m_float << ": " << (void*)&context.m_float << std::endl;
            std::cout << std::setw(10) << context.m_double << ": " << (void*)&context.m_double << std::endl;

            std::cout << std::endl << "[ptrs]" << std::endl;
            std::cout << std::setw(10) << (int) *context.m_ptr_int8 << ": " << (void*) context.m_ptr_int8 << std::endl;
            std::cout << std::setw(10) << *context.m_ptr_int16 << ": " << (void*) context.m_ptr_int16 << std::endl;
            std::cout << std::setw(10) << *context.m_ptr_int32 << ": " << (void*) context.m_ptr_int32 << std::endl;
            std::cout << std::setw(10) << *context.m_ptr_int64 << ": " << (void*) context.m_ptr_int64 << std::endl;
        }
        else if (command == "show") {
            show_values(pid, context);
        }
        else {
            std::cout << "ERROR: WRONG COMMAND." << std::endl;
        }
    }

    return 0;
}