cmake_minimum_required(VERSION 3.14)
project(UniversalBridge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. GESTIONE DIPENDENZE (Scarica librerie) ---
include(FetchContent)

# Asio (Networking)
message(STATUS "Scaricamento di Asio...")
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-28-0
)
FetchContent_MakeAvailable(asio)

# Simdjson (Parsing JSON)
message(STATUS "Scaricamento di Simdjson...")
FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG        v3.2.1 
)
FetchContent_MakeAvailable(simdjson)

# --- 2. CREAZIONE ESEGUIBILE ---
# Definiamo i file sorgenti
set(SOURCES
    src/main.cpp
    src/Engine.cpp
)

# CREA l'eseguibile "bridge"
add_executable(bridge ${SOURCES})

# --- 3. CONFIGURAZIONE DEL TARGET  ---

# Include la cartella locale 'include' del progetto
target_include_directories(bridge PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Include gli header di Asio (scaricati prima)
target_include_directories(bridge PRIVATE ${asio_SOURCE_DIR}/asio/include)

# Cerca e linka i Thread di sistema (necessari per Asio)
find_package(Threads REQUIRED)
target_link_libraries(bridge PRIVATE Threads::Threads)

# Linka la libreria Simdjson
target_link_libraries(bridge PRIVATE simdjson)

# Definisci la modalità Standalone per Asio (niente Boost)
target_compile_definitions(bridge PRIVATE ASIO_STANDALONE)