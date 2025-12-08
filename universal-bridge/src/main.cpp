#include "Engine.hpp"
#include <iostream>

int main() {
    try {
        std::cout << "=== Universal Data Stream Bridge v0.1 ===" << std::endl;
        
        Engine engine;
        engine.run();

    } catch (std::exception& e) {
        std::cerr << "Errore Fatale: " << e.what() << std::endl;
    }
    return 0;
}