#pragma once
#include <string>

 
struct ProductInfo {
    std::string ID;
    std::string name;
    double price;
    int stockQnty;

    std::string availability() const {
        if (stockQnty == 0)
            return "[!]OUT OF STOCK";
        else if (stockQnty <= 5)
            return "[!]Low Stock";
        return "In Stock";
    }

    bool operator==(const ProductInfo&) const = default;
};

struct CartItem {
    std::string ID;
    std::string name;
    double price;
    int orderQty;
    double total;
};


struct SaleReceipt {
    std::string transactionID; // e.g., "TXN-1001" to tie everything together
    std::string customerName;
    std::vector<CartItem> itemsBought; // Supports 1 to 100+ items seamlessly!
    double grandTotal;
    double customerPayment;
    double change;
    std::string date;          // e.g., "2026-06-15"
};


