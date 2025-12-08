#pragma once
#include <asio.hpp>
#include <simdjson.h> 
#include <iostream>
#include <memory>
#include <vector>

class Engine {
public:
    Engine();
    void run();

private:
    void start_accept();
    void read_data(std::shared_ptr<asio::ip::tcp::socket> socket);

    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    
    simdjson::ondemand::parser parser_;
};