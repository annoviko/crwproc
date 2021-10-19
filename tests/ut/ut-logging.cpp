/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "log/logging.h"

#include <filesystem>


TEST(ut_logging, start_stop) {
    log_writer * writer = new log_writer("test");

    ASSERT_FALSE(writer->get_filename().empty());
    ASSERT_TRUE(std::filesystem::exists(writer->get_filename()));

    delete writer;

    std::filesystem::remove(writer->get_filename());
}