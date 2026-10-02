#include "OrderBook.hpp"

int main(){

    // order ids stored here
    std::unordered_map<uint64_t, Order> id_list;

    OrderBook Apple("APPL");

    createOrder(Side::Buy, Apple, 15, 10025);
    createOrder(Side::Buy, Apple, 15, 10055);
    createOrder(Side::Buy, Apple, 15, 9015);
    createOrder(Side::Buy, Apple, 15, 12115);

    createOrder(Side::Sell, Apple, 15, 12225);
    createOrder(Side::Sell, Apple, 26, 12225);
    createOrder(Side::Sell, Apple, 15, 13000);
    createOrder(Side::Sell, Apple, 47, 13000);
    createOrder(Side::Sell, Apple, 15, 12500);
    createOrder(Side::Sell, Apple, 15, 14000);
    createOrder(Side::Sell, Apple, 15, 15000);

    Apple.printBook();

    // Testing
    createOrder(Side::Buy, Apple, 100, 16000);
    

    Apple.printBook();

    createOrder(Side::Sell, Apple, -35, 9015);

    Apple.printBook();

    
    for (int i = 0; i < 10'000'000; ++i){
        createOrder((Side)(rand() % 2), Apple, rand() % 150, rand() % 1000);
    }
    
    return 0;
}