
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

#include "orderSystem.h"
#include "inputHandling.h"
#include "display.h"
#include "errMsg.h"
#include "product.h"

// --- Private Helper Functions ---
// (No one outside this file can see or use these)
void addToCart(std::vector<ProductInfo>& products, std::vector<CartItem>& cart);
void removeFromCart(std::vector<CartItem>& cart);
void viewCart(const std::vector<CartItem>& cart);
char checkout(std::vector<CartItem>& cart);

// --- Main Public Function ---
void processOrder(std::vector<ProductInfo>& products){
    if (products.empty()) {
        std::cout << "\n[Notice] No products are available. Orders cannot be processed at this time.\n";
        return;
    }

    std::vector<CartItem> cart;
    char menuChoice;

    std::cout << std::fixed << std::setprecision(2);

    do {
        std::cout << "\n=== CUSTOMER ORDERING SYSTEM ===\n";
        std::cout << "a. Add Item to Cart\n";
        std::cout << "b. Remove Item from Cart\n";
        std::cout << "c. View Current Cart\n";
        std::cout << "d. Checkout & Complete Order\n";
        std::cout << "e. Cancel Order and Exit\n";
        
        menuChoice = getChar("Select an option (a-e): ", 'a', 'e');

        switch (menuChoice) {
            case 'a': // --- ADD TO CART ---
                displayProducts(products);
                addToCart(products, cart);
                break;

            case 'b':  // --- REMOVE FROM CART ---
                removeFromCart(cart);
                break;

            case 'c':  // --- VIEW CART ---
                viewCart(cart);
                break;

            case 'd':  // --- CHECKOUT ---
                menuChoice = checkout(cart);
                break;
                
            case 'e': // --- CANCEL ---
                std::cout << "\nOrder cancelled. Emptying cart and exiting module...\n";
                break;
        }

    } while (menuChoice != 'd' && menuChoice != 'e');
}

// ... implement your helper functions down here ...

void addToCart(std::vector<ProductInfo>& products, std::vector<CartItem>& cart){
    std::string targetID = getString("\nEnter Product ID to order ('cancel' to back out): ", 4, 6);
    if (targetID == "cancel") return;

    ProductInfo* targetProduct = nullptr;
    for (auto &p : products) {
        if (p.ID == targetID) {
            targetProduct = &p;
            break;
        }
    }

    if (targetProduct == nullptr) {
        std::cout << errmsg.productNotFound;
        return;
    }

    // Check how many are already sitting in the cart
    int alreadyInCart = 0;
    CartItem* existingCartItem = nullptr;
    for (auto &item : cart) {
        if (item.product->ID == targetID) {
            alreadyInCart = item.orderQty;
            existingCartItem = &item;
            break;
        }
    }

    int availableStock = targetProduct->stockQnty - alreadyInCart;
    if (availableStock <= 0) {
        std::cout << "\n[Error] \"" << targetProduct->name 
                    << "\" has no more available stock for allocation.\n";
        return;
    }

    std::cout << "\nProduct Found -> " << targetProduct->name
                << " | Price: " << targetProduct->price
                << " | Available Stock to Add: " << availableStock << "\n";

    int orderQty = getInt("Enter Quantity to add (0 to cancel): ", 0, availableStock);
    if (orderQty == 0) return;

    if (existingCartItem != nullptr) {
        existingCartItem->orderQty += orderQty;
    } else {
        cart.push_back({targetProduct, orderQty});
    }

    std::cout << "\nSuccessfully added " << orderQty << " x \"" << targetProduct->name << "\" to your cart!\n";

}

void removeFromCart(std::vector<CartItem>& cart){
    if (cart.empty()) {
        std::cout << "\n[Notice] Your cart is currently empty.\n";
        return;
    }

    std::string targetID = getString("\nEnter Product ID to remove ('cancel' to back out): ", 4, 6);
    if (targetID == "cancel") return;

    bool found = false;
    int i = 0;
    for(CartItem& ci : cart){
        if(targetID == ci.product->ID){
            found = true;
            break;
        }
        i++;
    }
    if(found){
        std::cout << "Removed all units of \"" << cart[i].product->name << "\" from the cart.\n";
    cart.erase(cart.begin() + i);
    }
    else {
        std::cout << "\n[Error] That item isn't in your cart.\n";
    }
}

void viewCart(const std::vector<CartItem>& cart){
    if (cart.empty()) {
        std::cout << "\n[Notice] Your cart is currently empty.\n";
    } else {
        double totalPrice = 0.0;
        for (const auto& item : cart) {
            totalPrice += (item.product->price * item.orderQty);
        }
        displayCart(cart, totalPrice);
    }
}

char checkout(std::vector<CartItem>& cart){
    if (cart.empty()) {
        std::cout << "\n[Error] Cannot checkout an empty cart!\n";
        return ' ';
    }

    std::cout << "\nProcessing payment and checking out...\n";

    // --- TRANSACTION / RECEIPT SUMMARY (50 Columns Wide) ---
    std::cout << "\n==================================================\n";
    std::cout << "               TRANSACTION RECEIPT                \n";
    std::cout << "==================================================\n";
    std::cout << std::left << std::setw(22) << "Item Description" 
              << std::right << std::setw(6) << "Qty" 
              << std::setw(11) << "Unit Price" 
              << std::setw(11) << "Total" << "\n";
    std::cout << "==================================================\n";

    double grandTotal = 0.0;

    for (CartItem& ci : cart) {
        double itemTotal = ci.product->price * ci.orderQty;
        grandTotal += itemTotal;

        // Truncate names longer than 21 characters to maintain column alignment
        std::string displayName = ci.product->name;
        if (displayName.length() > 21) {
            displayName = displayName.substr(0, 18) + "...";
        }

        std::cout << std::left << std::setw(22) << displayName
                  << std::right << std::setw(6) << ci.orderQty
                  << std::setw(11) << ci.product->price
                  << std::setw(11) << itemTotal << "\n";

        // Deduct stock from inventory
        ci.product->stockQnty -= ci.orderQty;
    }

    std::cout << "==================================================\n";
    std::cout << std::right << std::setw(39) << "total: " 
              << std::setw(11) << grandTotal << "\n";
    std::cout << "==================================================\n";

    return 'd';
}