#include "OrderBook.hpp"

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <utility>

std::uint64_t Order::current_id = 0;

Order::Order(std::uint64_t quantity, std::uint64_t value)
    : id(current_id++), quantity(quantity), value(value) {}

std::uint64_t Order::getId() const {
    return id;
}

std::uint64_t Order::getQuantity() const {
    return quantity;
}

void Order::adjustQuantity(int adjustment) {
    quantity = adjustment;
}

OrderBook::OrderBook(const std::string& ticker) : ticker(ticker) {}

void OrderBook::addOrder(Side side, Order& order, std::uint64_t value) {
    // Matching Logic
    int amount_left = order.getQuantity();

    if (side == Side::Buy) {
        auto it = asks.begin();
        // Traversal of the Orders at this level
        while (it != asks.end() && value >= it->first && amount_left > 0) {
            // remove level if empty
            if (it->second.size() == 0) {
                asks.erase(it++);
                continue;
            }

            // sweep price level
            amount_left -= it->second[0].getQuantity();
            if (amount_left >= 0) {
                it->second.erase(it->second.begin());
            } else {
                it->second[0].adjustQuantity(-1 * amount_left);
            }
        }
    } else if (side == Side::Sell) {
        auto it = bids.begin();
        while (it != bids.end() && value <= it->first && amount_left > 0) {
            if (it->second.size() == 0) {
                bids.erase(it++);
                continue;
            }

            amount_left -= it->second[0].getQuantity();
            if (amount_left >= 0) {
                it->second.erase(it->second.begin());
            } else {
                it->second[0].adjustQuantity(-1 * amount_left);
            }
        }
    }

    // Put remainder (if applicable) at a price level
    if (amount_left >= 0) {
        order.adjustQuantity(amount_left);
    } else {
        return;
    }

    if (side == Side::Buy) {
        bids[value].push_back(std::move(order));
    } else {
        asks[value].push_back(std::move(order));
    }
}

void OrderBook::cancelOrder(std::uint64_t id) {
    (void)id;
}

void OrderBook::printBook() {
    std::cout << ticker << std::endl << std::endl;

    std::cout << "ASK" << std::endl;
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::uint64_t lvl = it->first;
        std::uint64_t amt = 0;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            amt += it2->getQuantity();
        }
        std::cout << amt << " @ " << lvl << std::endl;
    }

    std::cout << "-----------" << std::endl;

    std::cout << "BID" << std::endl;
    for (auto it = bids.begin(); it != bids.end(); ++it) {
        std::uint64_t lvl = it->first;
        std::uint64_t amt = 0;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            amt += it2->getQuantity();
        }
        std::cout << amt << " @ " << lvl << std::endl;
    }
}

Order createOrder(Side side, OrderBook& book, std::uint64_t quantity, std::uint64_t value) {
    Order order(quantity, value);
    book.addOrder(side, order, value);
    return order;
}
