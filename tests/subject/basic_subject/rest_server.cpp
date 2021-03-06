/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "rest_server.h"

#include <sstream>
#include <string>


rest_server::rest_server(const std::string& p_address, const std::size_t p_port) :
    m_address(p_address),
    m_port(p_port)
{
    m_server.set_keep_alive_max_count(1);
    m_server.set_keep_alive_timeout(1);

    m_server.Get(R"(/address/memory/(\S+)/variable/(\S+))", [this](const httplib::Request& p_request, httplib::Response& p_response) {
        const std::string memory_type_name = p_request.matches[1];
        const std::string variable_type_name = p_request.matches[2];

        const memory_type mem_type = get_memory_type_from_string(memory_type_name);
        if (mem_type == memory_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided memory type '" + memory_type_name + "' does not exist.", "text/plain");
            return;
        }

        const value_type val_type = get_value_type_from_string(variable_type_name);
        if (val_type == value_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided variable type '" + variable_type_name + "' does not exist.", "text/plain");
            return;
        }

        const std::uint64_t address = m_manager.get_address_value(val_type, mem_type);

        std::stringstream stream;
        stream << std::hex << address;
        p_response.set_content(std::string(stream.str()), "text/plain");
        p_response.status = 200;
    });


    m_server.Get(R"(/address/sequence)", [this](const httplib::Request&, httplib::Response& p_response) {
        const std::uint64_t address = m_manager.get_address_sequence();

        std::stringstream stream;
        stream << std::hex << address;
        p_response.set_content(std::string(stream.str()), "text/plain");
        p_response.status = 200;
    });


    m_server.Get(R"(/memory/(\S+)/variable/(\S+))", [this](const httplib::Request& p_request, httplib::Response& p_response) {
        const std::string memory_type_name = p_request.matches[1];
        const std::string variable_type_name = p_request.matches[2];

        const memory_type mem_type = get_memory_type_from_string(memory_type_name);
        if (mem_type == memory_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided memory type '" + memory_type_name + "' does not exist.", "text/plain");
            return;
        }

        const value_type val_type = get_value_type_from_string(variable_type_name);
        if (val_type == value_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided variable type '" + variable_type_name + "' does not exist.", "text/plain");
            return;
        }

        const std::string value = m_manager.get_value(val_type, mem_type);
        p_response.set_content(value, "text/plain");
        p_response.status = 200;
    });


    m_server.Get(R"(/sequence)", [this](const httplib::Request&, httplib::Response& p_response) {
        std::stringstream stream;

        for (const std::uint8_t byte_value : m_manager.get_sequence()) {
            stream << std::hex << byte_value << " ";
        }

        p_response.set_content(stream.str(), "text/plain");
        p_response.status = 200;
    });


    m_server.Post(R"(/operation/(\S+)/memory/(\S+)/variable/(\S+)/value/(\S+))", [this](const httplib::Request& p_request, httplib::Response& p_response) {
        const std::string operation_type_name = p_request.matches[1];
        const std::string memory_type_name = p_request.matches[2];
        const std::string variable_type_name = p_request.matches[3];
        const std::string value = p_request.matches[4];

        const change_type chng_type = get_change_type_from_string(operation_type_name);
        if (chng_type == change_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided operation type '" + operation_type_name + "' does not exist.", "text/plain");
            return;
        }

        const memory_type mem_type = get_memory_type_from_string(memory_type_name);
        if (mem_type == memory_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided memory type '" + memory_type_name + "' does not exist.", "text/plain");
            return;
        }

        const value_type val_type = get_value_type_from_string(variable_type_name);
        if (val_type == value_type::invalid) {
            p_response.status = 404;
            p_response.set_content("Provided variable type '" + variable_type_name + "' does not exist.", "text/plain");
            return;
        }

        try {
            const bool status = m_manager.change_value(val_type, mem_type, chng_type, value);
            p_response.status = status ? 202 : 500;
        }
        catch (...) {
            p_response.set_content("Provided value is not convertible into specified type.", "text/plain");
            p_response.status = 400;
        }
    });


    m_server.Get(R"(/status)", [this](const httplib::Request&, httplib::Response& p_response) {
        p_response.set_content("Running.", "text/plain");
        p_response.status = 200;
    });


    m_server.Delete(R"(/application)", [this](const httplib::Request&, httplib::Response& p_response) {
        p_response.status = 200;
        m_server.stop();
    });
}


void rest_server::run() {
    m_server.listen(m_address.c_str(), static_cast<int>(m_port));
}
