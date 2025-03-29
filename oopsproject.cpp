#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For formatting output
#include <cstdlib> // For random price generation
#include <ctime>   // For random seed

using namespace std;

// Class representing a single stock
class Stock {
private:
    string symbol;
    string companyName;
    double price;

public:
    Stock(string sym, string name, double p) : symbol(sym), companyName(name), price(p) {}

    string getSymbol() const { return symbol; }
    string getCompanyName() const { return companyName; }
    double getPrice() const { return price; }

    void updatePrice() {
        // Simulate price fluctuation (-5% to +5%)
        double change = ((rand() % 11) - 5) / 100.0;
        price += price * change;
        if (price < 1.0) price = 1.0; // Ensure price doesn't drop below 1
    }

    void display() const {
        cout << left << setw(10) << symbol << setw(20) << companyName 
             << "$" << fixed << setprecision(2) << price << endl;
    }
};

// Class representing a user's portfolio
class Portfolio {
private:
    struct StockHolding {
        string symbol;
        int quantity;
        double averagePrice;
    };

    vector<StockHolding> holdings;
    double balance;

public:
    Portfolio(double initialBalance) : balance(initialBalance) {}

    void buyStock(const Stock& stock, int quantity) {
        double cost = stock.getPrice() * quantity;

        if (cost > balance) {
            cout << "Insufficient balance to complete the transaction." << endl;
            return;
        }

        // Check if stock is already in the portfolio
        for (auto& holding : holdings) {
            if (holding.symbol == stock.getSymbol()) {
                holding.averagePrice = ((holding.averagePrice * holding.quantity) + cost) 
                                       / (holding.quantity + quantity);
                holding.quantity += quantity;
                balance -= cost;
                cout << "Bought " << quantity << " shares of " << stock.getSymbol() << endl;
                return;
            }
        }

        // Add a new stock to the portfolio
        holdings.push_back({stock.getSymbol(), quantity, stock.getPrice()});
        balance -= cost;
        cout << "Bought " << quantity << " shares of " << stock.getSymbol() << endl;
    }

 void sellStock(const Stock& stock, int quantity) {
    for (auto it = holdings.begin(); it != holdings.end(); ++it) {
        if (it->symbol == stock.getSymbol()) { // Found the stock in the portfolio
            if (quantity > it->quantity) {
                cout << "You don't own enough shares to sell." << endl;
                return;
            }

            double earnings = stock.getPrice() * quantity;
            it->quantity -= quantity; // Reduce the quantity of shares
            balance += earnings; // Add earnings to the user's balance
            cout << "Sold " << quantity << " shares of " << stock.getSymbol() << endl;

            // Remove stock from portfolio if no shares are left
            if (it->quantity == 0) {
                holdings.erase(it); // Safely erase the element
            }
            return; // Exit after processing
        }
    }

    cout << "You don't own this stock." << endl; // Stock not found in portfolio
}



    void displayPortfolio() const {
        cout << "\nYour Portfolio:\n";
        cout << left << setw(10) << "Symbol" << setw(10) << "Quantity" 
             << setw(15) << "Avg Price" << endl;
        for (const auto& holding : holdings) {
            cout << left << setw(10) << holding.symbol << setw(10) << holding.quantity 
                 << "$" << fixed << setprecision(2) << holding.averagePrice << endl;
        }
        cout << "Balance: $" << fixed << setprecision(2) << balance << "\n" << endl;
    }

    double getBalance() const { return balance; }
};

// Main function to run the simulator
int main() {
    srand(time(0)); // Seed for random number generation

    // Initialize stock market
    vector<Stock> market = {
        Stock("AAPL", "Apple Inc.", 150.00),
        Stock("GOOGL", "Alphabet Inc.", 2800.00),
        Stock("TSLA", "Tesla Inc.", 700.00),
        Stock("AMZN", "Amazon Inc.", 3300.00)
    };

    // Initialize user's portfolio
    Portfolio userPortfolio(10000.00); // Starting balance

    int choice;
    do {
        // Display menu
        cout << "\n--- Stock Market Simulator ---\n";
        cout << "1. View Market\n";
        cout << "2. Buy Stock\n";
        cout << "3. Sell Stock\n";
        cout << "4. View Portfolio\n";
        cout << "5. Simulate Price Fluctuation\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // View market
                cout << "\nStock Market:\n";
                cout << left << setw(10) << "Symbol" << setw(20) << "Company" 
                     << setw(10) << "Price" << endl;
                for (const auto& stock : market) {
                    stock.display();
                }
                break;

            case 2: { // Buy stock
                string symbol;
                int quantity;
                cout << "Enter stock symbol: ";
                cin >> symbol;
                cout << "Enter quantity: ";
                cin >> quantity;

                bool found = false;
                for (const auto& stock : market) {
                    if (stock.getSymbol() == symbol) {
                        userPortfolio.buyStock(stock, quantity);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Stock not found in the market.\n";
                }
                break;
            }

            case 3: { // Sell stock
                string symbol;
                int quantity;
                cout << "Enter stock symbol: ";
                cin >> symbol;
                cout << "Enter quantity: ";
                cin >> quantity;

                bool found = false;
                for (const auto& stock : market) {
                    if (stock.getSymbol() == symbol) {
                        userPortfolio.sellStock(stock, quantity);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Stock not found in the market.\n";
                }
                break;
            }

            case 4: // View portfolio
                userPortfolio.displayPortfolio();
                break;

            case 5: // Simulate price fluctuation
                for (auto& stock : market) {
                    stock.updatePrice();
                }
                cout << "Stock prices updated.\n";
                break;

            case 6: // Exit
                cout << "Exiting Stock Market Simulator. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
