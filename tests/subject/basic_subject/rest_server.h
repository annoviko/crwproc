#pragma once

#include <string>

#include <cpphttplib/httplib.h>

#include "memory_manager.h"


class rest_server {
private:
    httplib::Server m_server;
    std::string m_address;
    std::size_t m_port;

    memory_manager m_manager;


public:
    rest_server(const std::string& p_address, const std::size_t p_port);

public:
    void run();
};