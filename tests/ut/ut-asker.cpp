/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <future>

#include "control/asker.h"
#include "control/command_interrupt.h"
#include "control/event.h"



class ut_asker : public ::testing::Test {
private:
    std::streambuf* m_original_cout_stream = std::cout.rdbuf();
    std::streambuf* m_original_cin_stream = std::cin.rdbuf();

    std::stringstream m_output_stream;
    std::stringstream m_input_stream;

protected:
    void SetUp() override {
        m_output_stream = std::stringstream();
        m_input_stream = std::stringstream();

        std::cout.rdbuf(m_output_stream.rdbuf());
        std::cin.rdbuf(m_input_stream.rdbuf());
    }

    void TearDown() override {
        m_output_stream.clear();
        m_input_stream.clear();

        std::cout.rdbuf(m_original_cout_stream);
        std::cin.rdbuf(m_original_cin_stream);
    }

    void RunTestAskIndex(const std::string p_input, const std::size_t p_expected_value, const std::size_t p_limit, const bool p_exception_expected = false) {
        SetUp();

        m_input_stream << p_input;

        if (p_exception_expected) {
            bool interrupted = false;
            try {
                const std::size_t actual_value = asker::ask_index(p_limit, true).get_begin();
            }
            catch (command_interrupt&) {
                interrupted = true;
            }

            ASSERT_TRUE(interrupted);
        }
        else {
            std::shared_ptr<std::future<bool>> future_ptr = nullptr;
            if (p_expected_value == asker::INVALID_INDEX) {
                future_ptr = std::make_shared<std::future<bool>>(std::async(std::launch::async, [this]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    m_input_stream << "\n";
                    return true;
                }));
            }

            const index_info info = asker::ask_index(p_limit, true);
            if (p_expected_value != asker::INVALID_INDEX) {
                const std::size_t actual_value = info.get_begin();
                ASSERT_EQ(p_expected_value, actual_value);
            }
            else {
                ASSERT_FALSE(info.is_valid());
            }

            if (future_ptr) {
                ASSERT_TRUE(future_ptr->get());
            }
        }
    }
};


TEST_F(ut_asker, index_correct) {
    RunTestAskIndex("5", 5, 10, false);
}


TEST_F(ut_asker, index_correct_full_range) {
    for (std::size_t i = 0; i < 10; i++) {
        RunTestAskIndex(std::to_string(i), i, 10, false);
    }
}


TEST_F(ut_asker, index_out_of_range) {
    RunTestAskIndex("10", asker::INVALID_INDEX, 10, false);
    RunTestAskIndex("500", asker::INVALID_INDEX, 10, false);
}


TEST_F(ut_asker, user_command) {
    RunTestAskIndex("\\choose", asker::INVALID_INDEX, 10, true);
}


TEST_F(ut_asker, invalid_input) {
    RunTestAskIndex("invalid_input", asker::INVALID_INDEX, 10, false);
}
