#include "Engine.hpp"

using asio::ip::tcp;

Engine::Engine() 
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), 8080)) {
    std::cout << "[Engine] Inizializzato su porta 8080. (Simdjson Ready)" << std::endl;
}

void Engine::run() {
    start_accept();
    std::cout << "[Engine] Loop avviato. In attesa di dati JSON..." << std::endl;
    io_context_.run();
}

void Engine::start_accept() {
    auto socket = std::make_shared<tcp::socket>(io_context_);

    acceptor_.async_accept(*socket, [this, socket](std::error_code ec) {
        if (!ec) {
            std::cout << "[Network] Connessione! Leggo i dati..." << std::endl;
            read_data(socket); 
        } else {
            std::cerr << "[Error] Accept failed: " << ec.message() << std::endl;
        }
        start_accept();
    });
}

void Engine::read_data(std::shared_ptr<tcp::socket> socket) {
    auto buffer = std::make_shared<std::vector<char>>(4096);

    socket->async_read_some(asio::buffer(*buffer), 
        [this, socket, buffer](std::error_code ec, std::size_t length) {
        if (!ec) {
            simdjson::padded_string json_data(buffer->data(), length);
            simdjson::ondemand::document doc;
            
            auto error = parser_.iterate(json_data).get(doc);
            
            if (!error) {
                std::cout << "✅ [JSON VALID] Dati ricevuti (" << length << " bytes):" << std::endl;
                
                std::cout << std::string_view(json_data.data(), json_data.size()) << std::endl;

            } else {
                std::cerr << "⚠️ [INVALID] Errore: " << simdjson::error_message(error) << std::endl;
                std::cout << "Raw: " << std::string_view(buffer->data(), length) << std::endl;
            }

            read_data(socket);
            
        } else {
            if (ec != asio::error::eof)
                std::cerr << "[Error] Read failed: " << ec.message() << std::endl;
        }
    });
}