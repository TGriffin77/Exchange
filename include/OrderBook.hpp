#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

enum Side {
    Buy,
    Sell
};

class Order {
private:
    std::uint64_t id;
    static std::uint64_t current_id;
    std::uint64_t quantity;
    std::uint64_t value;

public:
    Order(std::uint64_t quantity, std::uint64_t value);

    std::uint64_t getId() const;
    std::uint64_t getQuantity() const;
    void adjustQuantity(int adjustment);
};

class OrderBook {
private:
    std::string ticker;

    // Bids are ordered from highest to lowest; asks are ordered from lowest to highest.
    std::map<std::uint64_t, std::vector<Order>, std::greater<std::uint64_t>> bids;
    std::map<std::uint64_t, std::vector<Order>> asks;

public:
    explicit OrderBook(const std::string& ticker);

    void addOrder(Side side, Order& order, std::uint64_t value);
    void cancelOrder(std::uint64_t id);
    void printBook();
};

Order createOrder(Side side, OrderBook& book, std::uint64_t quantity, std::uint64_t value);
