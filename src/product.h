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
            return "Low Stock";
        return "In Stock";
    }

    bool operator==(const ProductInfo&) const = default;
};

struct SaleReceipt {
    std::string customerName;
    std::string productID;
    double productPrice;
    int qtyBought;
    double total;
    double customerPayment;
    double change;

};

struct CartItem {
    ProductInfo* product;
    int orderQty;
};
