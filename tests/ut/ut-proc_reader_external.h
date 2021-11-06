/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>

#include <cpphttplib/httplib.h>
#include <gtest/gtest.h>

#include "core/proc_reader.h"
#include "core/proc_table.h"

#include "ut-proc_reader.h"


class ut_proc_reader_external : public ::testing::Test {
protected:
    STARTUPINFO m_proc_startup_info;
    PROCESS_INFORMATION m_proc_info;
    std::shared_ptr<httplib::Client> m_client = nullptr;

protected:
    void SetUp() override {
        std::memset(&m_proc_startup_info, 0x00, sizeof(m_proc_startup_info));
        std::memset(&m_proc_info, 0x00, sizeof(m_proc_info));

        char arguments[] = "basic_subject.exe --silent";
        if (!CreateProcess("basic_subject.exe", arguments, NULL, NULL, FALSE, 0, NULL, NULL, &m_proc_startup_info, &m_proc_info)) {
            FAIL() << "Impossible to start basic_subject process (reason: '" << GetLastError() << "')";
        }

        m_client = std::make_shared<httplib::Client>("127.0.0.1", 51425);
        httplib::Result result = m_client->Get("/status");
        EXPECT_EQ(200, result->status);
    }


    void TearDown() override {
        m_client->Delete("/application");
        m_client->stop();

        WaitForSingleObject(m_proc_info.hProcess, 1000);
        CloseHandle(m_proc_info.hProcess);
        CloseHandle(m_proc_info.hThread);
    }


    void IncreaseValue(const std::string& p_memory, const std::string& p_variable) const {
        std::stringstream stream;
        stream << "/operation/add/memory/" << p_memory << "/variable/" << p_variable << "/value/1";
        httplib::Result result = m_client->Post(stream.str().c_str());
        EXPECT_EQ(202, result->status);
    }


    template <typename TypeValue>
    TypeValue ReadValue(const std::string& p_memory, const std::string& p_variable) const {
        std::stringstream stream;
        stream << "/memory/" << p_memory << "/variable/" << p_variable;
        httplib::Result result = m_client->Get(stream.str().c_str());

        EXPECT_EQ(200, result->status);

        if constexpr (std::is_integral_v<TypeValue>) {
            if constexpr (std::is_signed_v<TypeValue>) {
                return static_cast<TypeValue>(std::stoll(result->body));
            }
            else {
                return static_cast<TypeValue>(std::stoull(result->body));
            }
        }
        else if (std::is_same_v<TypeValue, float>) {
            return static_cast<TypeValue>(std::stof(result->body));
        }
        else if (std::is_same_v<TypeValue, double>) {
            return static_cast<TypeValue>(std::stod(result->body));
        }
        else {
            throw std::exception("Impossible to extract numerical value.");
        }
    }


    std::uint64_t ReadAddress(const std::string& p_memory, const std::string& p_variable) const {
        std::stringstream stream;
        stream << "/address/memory/" << p_memory << "/variable/" << p_variable;
        httplib::Result result = m_client->Get(stream.str().c_str());

        EXPECT_EQ(200, result->status);

        const std::string address_as_string = result->body;
        return std::stoull(address_as_string, nullptr, 16);
    }

    template <typename TypeValue>
    static std::string GetType() {
        if constexpr (std::is_same_v<TypeValue, std::int8_t>) {
            return "i8";
        }
        else if constexpr (std::is_same_v<TypeValue, std::int16_t>) {
            return "i16";
        }
        else if constexpr (std::is_same_v<TypeValue, std::int32_t>) {
            return "i32";
        }
        else if constexpr (std::is_same_v<TypeValue, std::int64_t>) {
            return "i64";
        }
        else if constexpr (std::is_same_v<TypeValue, std::uint8_t>) {
            return "u8";
        }
        else if constexpr (std::is_same_v<TypeValue, std::uint16_t>) {
            return "u16";
        }
        else if constexpr (std::is_same_v<TypeValue, std::uint32_t>) {
            return "u32";
        }
        else if constexpr (std::is_same_v<TypeValue, std::uint64_t>) {
            return "u64";
        }
        else if constexpr (std::is_same_v<TypeValue, float>) {
            return "flt";
        }
        else if constexpr (std::is_same_v<TypeValue, double>) {
            return "dbl";
        }
    }


    template <typename TypeValue>
    static bool IsSigned() {
        if constexpr (std::is_signed<TypeValue>::value) {
            return true;
        }
        else {
            return false;
        }
    }


    template <typename TypeValue>
    void test_template_external_find_value(const std::string& p_memory) {
        const proc_info info = proc_table().get().at(static_cast<std::size_t>(m_proc_info.dwProcessId));

        const std::string variable_type = ut_proc_reader_external::GetType<TypeValue>();

        filter_equal filter = filter_equal(type_desc(value_type::integral, sizeof(TypeValue), IsSigned<TypeValue>()));
        filter.set_value(ReadValue<TypeValue>(p_memory, variable_type));

        const std::uint64_t expected_address = ReadAddress(p_memory, variable_type);

        search_result result = proc_reader(info, filter).read_and_filter();

        ASSERT_FALSE(result.is_empty());
        ASSERT_TRUE(contains_address(result, (void*)expected_address));

        const std::size_t attempts_limit = 10;
        for (std::size_t i = 0; (i < attempts_limit) && (result.get_amount_values() > 1); i++) {
            IncreaseValue(p_memory, variable_type);

            filter_equal updated_filter = filter_equal(type_desc(value_type::integral, sizeof(TypeValue), IsSigned<TypeValue>()));
            updated_filter.set_value(ReadValue<TypeValue>(p_memory, variable_type));

            result = proc_reader(info, updated_filter).read_and_filter(result);

            ASSERT_FALSE(result.is_empty());
        }

        ASSERT_FALSE(result.is_empty());

        const TypeValue expected_value = ReadValue<TypeValue>(p_memory, variable_type);
        for (const auto& block : result.get_memory_blocks()) {
            for (const auto& pointer : block.get_values()) {
                if (pointer.get_address() == expected_address) {
                    ASSERT_EQ(expected_value, pointer.get_value().get<TypeValue>());
                    return;
                }
            }
        }

        FAIL() << "Value is not found";
    }
};
