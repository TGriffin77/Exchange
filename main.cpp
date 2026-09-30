#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>

enum Side {
    Buy,
    Sell
};

class Order{
private:
    uint64_t id;
    inline static uint64_t current_id = 0;
    uint64_t quantity;
    uint64_t value;
    
public:
    Order(uint64_t quantity, uint64_t value) : id(current_id++), quantity(quantity), value(value){};

    uint64_t getId() const {
        return id;
    }

    uint64_t getQuantity() const {
        return quantity;
    }

    void adjustQuantity(int adjustment) {
        quantity = adjustment;
    }
};

class OrderBook{
private:
    std::string ticker;

    //!! These might be better inversed (bids is normal and asks is greater)
    std::map<uint64_t, std::vector<Order>, std::greater<uint64_t>> bids;
    std::map<uint64_t, std::vector<Order>> asks;

public:
    OrderBook(const std::string &_ticker): ticker(_ticker){};

    void addOrder(Side side, Order& order, uint64_t value){
        // Matching Logic
        int amount_left = order.getQuantity();

        if(side == Side::Buy){
            auto it = asks.begin();
            // Traversal of the Orders at this level
            while (it != asks.end() && value >= it->first && amount_left > 0){
                // remove level if empty
                if (it->second.size() == 0){
                    asks.erase(it++);
                    continue;
                }

                // sweep price level
                amount_left -= it->second[0].getQuantity();
                if (amount_left >= 0){
                    it->second.erase(it->second.begin());
                }
                else{
                    it->second[0].adjustQuantity(-1*amount_left);
                }
            }
        }
        else if(side == Side::Sell){
            auto it = bids.begin();
            while (it != bids.end() && value <= it->first && amount_left > 0){
                if (it->second.size() == 0){
                    bids.erase(it++);
                    continue;
                }

                amount_left -= it->second[0].getQuantity();
                if(amount_left >= 0){
                    it->second.erase(it->second.begin());
                }
                else{
                    it->second[0].adjustQuantity(-1*amount_left);
                }
            }
        }

        // Put remainder (if applicable) at a price level
        if (amount_left >= 0) order.adjustQuantity(amount_left); 
        else return;
        

        if (side == Side::Buy){
            bids[value].push_back(std::move(order));
        }
        else{
            asks[value].push_back(std::move(order));
        }
    }

    void cancelOrder(uint64_t id){

    }

    void printBook(){
        std::cout << ticker << std::endl << std::endl;
        
        std::cout << "ASK" << std::endl;
        for (auto it = asks.rbegin(); it != asks.rend(); ++it){
            uint64_t lvl = it->first;
            uint64_t amt = 0;
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
                amt += it2->getQuantity();
            }
            std::cout << amt << " @ " << lvl << std::endl;
        }

        std::cout << "-----------" << std::endl;

        std::cout << "BID" << std::endl;
        for (auto it = bids.begin(); it != bids.end(); ++it){
            uint64_t lvl = it->first;
            uint64_t amt = 0;
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
                amt += it2->getQuantity();
            }
            std::cout << amt << " @ " << lvl << std::endl;
        }
    }
};

Order createOrder(Side side, OrderBook &book, uint64_t quantity, uint64_t value){
        Order order(quantity, value);
        book.addOrder(side, order, value);

        return order;
};

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