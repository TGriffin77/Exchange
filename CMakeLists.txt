cmake_minimum_required(VERSION 3.16)

project(OrderBook LANGUAGES CXX)

add_library(orderbook_core
    src/OrderBook.cpp
)

target_include_directories(orderbook_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(orderbook_core PUBLIC cxx_std_17)
set_target_properties(orderbook_core PROPERTIES CXX_EXTENSIONS OFF)

add_executable(orderbook
    src/main.cpp
)
target_link_libraries(orderbook PRIVATE orderbook_core)
set_target_properties(orderbook PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC)
    target_compile_options(orderbook_core PRIVATE /W4)
    target_compile_options(orderbook PRIVATE /W4)
else()
    target_compile_options(orderbook_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(orderbook PRIVATE -Wall -Wextra -Wpedantic)
endif()
