// order_book.cpp
#include "order_book.hpp"
#include <algorithm>
#include <iostream>

OrderBook::OrderBook()
    : buyOrders(std::make_unique<PrioritizableValueST<std::string, Order>>())
    , sellOrders(std::make_unique<PrioritizableValueST<std::string, Order>>())
{}

OrderBook::~OrderBook() = default;

void OrderBook::addOrder(Order& order) {
    if (order.getQuantity() <= 0) {
        return;  // Ignore orders with zero or negative quantity
    }

    auto& orders = order.isBuyOrder() ? buyOrders : sellOrders;
    orders->put(order.getOrderId(), 
                order.getPrice(), 
                order.getTimestamp(), 
                order,
                order.isBuyOrder());
}

int OrderBook::getQuantityAtPrice(double price, bool isBuyOrder) const {
    int totalQuantity = 0;
    const auto& orders = isBuyOrder ? buyOrders : sellOrders;
    
    for (const auto& orderRef : orders->getAllValues()) {
        const auto& order = orderRef.get();
        if (order.getPrice() == price && !order.isCanceled()) {
            totalQuantity += order.getRemainingQuantity();
        }
    }
    return totalQuantity;
}

void OrderBook::cancelOrder(const std::string& orderId, bool isBuyOrder) {
    auto& orders = isBuyOrder ? buyOrders : sellOrders;
    if (auto orderOpt = orders->get(orderId)) {
        Order& order = orderOpt->get();
        order.cancel();
        // Re-insert the canceled order to maintain proper ordering
        reinsertOrder(order, isBuyOrder);
    }
}

bool OrderBook::isMatchPossible(const Order& buyOrder, const Order& sellOrder) const {
    // Add an explicit check for different order types
    if (buyOrder.isBuyOrder() == sellOrder.isBuyOrder()) {
        std::cout << "Attempted to match orders of the same type: "
                  << buyOrder.getOrderId() << " and " << sellOrder.getOrderId() << std::endl;
        return false;
    }

    return !buyOrder.isCanceled() && 
           !sellOrder.isCanceled() && 
           buyOrder.getRemainingQuantity() > 0 && 
           sellOrder.getRemainingQuantity() > 0 && 
           buyOrder.getPrice() >= sellOrder.getPrice() &&
           buyOrder.getStockSymbol() == sellOrder.getStockSymbol();
}

void OrderBook::reinsertOrder(Order& order, bool isBuyOrder) {
    auto& orders = isBuyOrder ? buyOrders : sellOrders;
    orders->put(order.getOrderId(), 
                order.getPrice(), 
                order.getTimestamp(), 
                order,
                order.isBuyOrder());
}

void OrderBook::processMatch(Order& buyOrder, Order& sellOrder) {
    if (!isMatchPossible(buyOrder, sellOrder)) {
        std::cout << "No match possible for " << buyOrder.getOrderId()
                  << " and " << sellOrder.getOrderId() << std::endl;
        return;
    }

    int matchQuantity = std::min(buyOrder.getRemainingQuantity(), sellOrder.getRemainingQuantity());
    
    std::cout << "Matching " << matchQuantity << " units between "
              << buyOrder.getOrderId() << " and " << sellOrder.getOrderId() << std::endl;

    buyOrder.reduceQuantity(matchQuantity);
    sellOrder.reduceQuantity(matchQuantity);

    // Debug output after reduction
    std::cout << "After reduction: " << buyOrder.getOrderId() 
              << " remaining: " << buyOrder.getRemainingQuantity() << ", "
              << sellOrder.getOrderId() << " remaining: " << sellOrder.getRemainingQuantity() << std::endl;

    // Reinsertion only if there is remaining quantity
    if (buyOrder.getRemainingQuantity() > 0) {
        std::cout << "Reinserting buy order with remaining quantity: " 
                  << buyOrder.getRemainingQuantity() << std::endl;
        reinsertOrder(buyOrder, true);
    }
    if (sellOrder.getRemainingQuantity() > 0) {
        std::cout << "Reinserting sell order with remaining quantity: " 
                  << sellOrder.getRemainingQuantity() << std::endl;
        reinsertOrder(sellOrder, false);
    }
}

bool OrderBook::isOrderCanceled(const std::string& orderId, bool isBuyOrder) const {
    auto& orders = isBuyOrder ? buyOrders : sellOrders;
    auto orderOpt = orders->get(orderId);
    return !orderOpt || orderOpt->get().isCanceled();
}

void OrderBook::matchOrders() {
    bool madeMatch;
    do {
        madeMatch = false;
        if (buyOrders->isEmpty() || sellOrders->isEmpty()) {
            break;
        }

        // Get highest priority buy order
        auto buyOrderOpt = buyOrders->deleteMin();
        if (!buyOrderOpt) {
            break;
        }

        Order& buyOrder = buyOrderOpt->get();
        if (buyOrder.isCanceled() || buyOrder.getRemainingQuantity() <= 0) {
            std::cout << "Skipping canceled or zero-quantity buy order: " << buyOrder.getOrderId() << "\n";
            continue;
        }

        // Get highest priority sell order
        auto sellOrderOpt = sellOrders->deleteMin();
        if (!sellOrderOpt) {
            reinsertOrder(buyOrder, true);
            break;
        }

        Order& sellOrder = sellOrderOpt->get();
        if (sellOrder.isCanceled() || sellOrder.getRemainingQuantity() <= 0) {
            std::cout << "Skipping canceled or zero-quantity sell order: " << sellOrder.getOrderId() << "\n";
            reinsertOrder(buyOrder, true);
            continue;
        }

        // Proceed with matching
        if (!isMatchPossible(buyOrder, sellOrder)) {
            std::cout << "No match possible for " << buyOrder.getOrderId() << " and " << sellOrder.getOrderId() << "\n";
            reinsertOrder(buyOrder, true);
            reinsertOrder(sellOrder, false);
            break;
        }

        // We have a match, process it
        processMatch(buyOrder, sellOrder);
        madeMatch = true;

    } while (madeMatch);
}

