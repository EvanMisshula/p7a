#include <iostream>
#include <deque>


class Order {

public:

    virtual ~Order() {}

    virtual void execute() = 0; // Executes the order

    virtual std::string getOrderDetails() const = 0; // Returns details of the order

};

class OrderManager {

public:

    virtual ~OrderManager() {}

    virtual void sendOrder(Order* order) = 0; // Sends the order to the market simulator

    virtual void receiveExecutionReport(const std::string& report) = 0; // Receives execution feedback

};

class TradingStrategy {

public:

    virtual ~TradingStrategy() {}

    virtual void generateSignal() = 0; // Generates a buy/sell signal

    virtual void sendOrderToManager(OrderManager* orderManager) = 0; // Sends order to Order Manager

};

class MarketSimulator {

public:

    virtual ~MarketSimulator() {}

    virtual void simulateMarket(OrderManager* orderManager) = 0; // Simulates market behavior, fills or rejects orders

};

class LimitOrder : public Order {

private:

    std::string symbol;

    int quantity;

    double price;

public:

    LimitOrder(const std::string& sym, int qty, double prc) : symbol(sym), quantity(qty), price(prc) {}

    

    void execute() override {

        std::cout << "Executing Limit Order: " << symbol << " for " << quantity << " at " << price << std::endl;

    }

    

    std::string getOrderDetails() const override {

        return "LimitOrder: " + symbol + " Qty: " + std::to_string(quantity) + " Price: " + std::to_string(price);

    }

};

class SimpleOrderManager : public OrderManager {

private:

    std::deque<Order*> ordersQueue; // Deque to hold orders

public:

    void sendOrder(Order* order) override {

        ordersQueue.push_back(order);

        std::cout << "Order sent to the market: " << order->getOrderDetails() << std::endl;

    }

 

    void receiveExecutionReport(const std::string& report) override {

        std::cout << "Execution report received: " << report << std::endl;

    }

 

    Order* getNextOrder() {

        if (!ordersQueue.empty()) {

            Order* order = ordersQueue.front();

            ordersQueue.pop_front();

            return order;

        }

        return nullptr;

    }

};

 


class SimpleTradingStrategy : public TradingStrategy {

public:

    void generateSignal() override {

        std::cout << "Generating Buy Signal..." << std::endl;

    }

 

    void sendOrderToManager(OrderManager* orderManager) override {

        Order* order = new LimitOrder("AAPL", 100, 150.50); // Example order

        orderManager->sendOrder(order);

    }

};

class SimpleMarketSimulator : public MarketSimulator {

public:

    void simulateMarket(OrderManager* orderManager) override {

        // Simulating market by fetching the next order from the order manager and executing it

        Order* order = orderManager->getNextOrder();

        if (order != nullptr) {

            order->execute();

            orderManager->receiveExecutionReport("Order filled for " + order->getOrderDetails());

        } else {

            std::cout << "No orders to execute in the market." << std::endl;

        }

    }

};

int main() {

    // Create components

    SimpleOrderManager orderManager;

    SimpleTradingStrategy strategy;

    SimpleMarketSimulator marketSimulator;

    

    // Step 1: Strategy generates signal and sends order to OrderManager

    strategy.generateSignal();

    strategy.sendOrderToManager(&orderManager);

    

    // Step 2: MarketSimulator executes the order and sends back an execution report

    marketSimulator.simulateMarket(&orderManager);

    

    return 0;

}
