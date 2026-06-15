#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

#include "orderSystem.h"
#include "inputHandling.h"
#include "display.h"
#include "errMsg.h"
#include "product.h"
#include "fileHandling.h"

// --- Private Helper Functions ---
// (No one outside this file can see or use these)
void addToCart(std::vector<ProductInfo>& products, std::vector<CartItem>& cart);
void removeFromCart(std::vector<CartItem>& cart);
void viewCart(const std::vector<CartItem>& cart);
char checkout(std::vector<CartItem>& cart, std::vector<ProductInfo>& inventory, std::vector<SaleReceipt>& salesLog);
std::string generateNextTransactionID(const std::vector<SaleReceipt>& logs);
std::string getCurrentDate();

// --- Main Public Function ---
void processOrder(std::vector<ProductInfo>& products, std::vector<SaleReceipt>& salesLog){
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
                menuChoice = checkout(cart, products, salesLog);
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
        if (item.ID == targetID) {
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
        cart.push_back({targetProduct->ID, targetProduct->name, targetProduct->price, orderQty, orderQty*targetProduct->price});
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
        if(targetID == ci.ID){
            found = true;
            break;
        }
        i++;
    }
    if(found){
        std::cout << "Removed all units of \"" << cart[i].name << "\" from the cart.\n";
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
            totalPrice += (item.total);
        }
        displayCart(cart, totalPrice);
    }
}

char checkout(std::vector<CartItem>& cart, std::vector<ProductInfo>& inventory, std::vector<SaleReceipt>& salesLog) {
    if (cart.empty()) {
        std::cout << "\n[Error] Cannot checkout an empty cart!\n";
        return ' ';
    }

    std::cout << "\nProcessing payment and checking out...\n";
    
    double grandTotal = 0.0;
    
    // --- PROCESS AND DISPLAY ITEMS ---
    printReceiptHeader();

    for (CartItem& ci : cart) {
        grandTotal += ci.total;
        printReceiptItem(ci);

        // Deduct stock from inventory
        ProductInfo* targetProduct = nullptr;
        for (auto &p : inventory) {
            if (p.ID == ci.ID) {
                targetProduct = &p;
                break;
            }
        }
        
        // Safety check to prevent crashes if a product ID doesn't match
        if (targetProduct != nullptr) {
            targetProduct->stockQnty -= ci.orderQty;
        }
    }

    std::cout << "==================================================\n";
    std::cout << std::right << std::setw(39) << "Total: " 
              << std::setw(11) << grandTotal << "\n";
    std::cout << "==================================================\n";

    // --- TRANSACTION METADATA GENERATION ---
    std::string transactionID = generateNextTransactionID(salesLog);
    std::string customerName = getString("Enter customer name : ", 1);
    
    // --- PAYMENT HANDLING & VALIDATION LOOP ---
    double payment = 0.0;
    while (true) {
        payment = getDouble("Enter customer payment: ", 0.01);
        
        if (payment >= grandTotal) {
            break; // Valid payment received, break the loop
        }
        
        std::cout << "\n[Error] Insufficient payment! Current total is " << grandTotal 
                  << ". Please enter an amount equal to or greater.\n";
    }

    // Calculate final change
    double change = payment - grandTotal;

    // --- PRINT PAYMENT SECTION OF RECEIPT ---
    std::cout << std::right << std::setw(39) << "Payment: " 
              << std::setw(11) << payment << "\n";
    std::cout << std::right << std::setw(39) << "Change: " 
              << std::setw(11) << change << "\n";
    std::cout << "==================================================\n";
    std::cout << "  Thank you for shopping, " << customerName << "!\n";
    std::cout << "  Transaction ID: " << transactionID << "\n";
    std::cout << "==================================================\n\n";

    // --- RECORD RETRIEVAL & RETENTION ---
    std::string currentDate = getCurrentDate(); // Grab local OS time/date

    // Build the structural database receipt object
    SaleReceipt receipt = {
        transactionID,
        customerName,
        cart,          // Copies items bought directly inside
        grandTotal,
        payment,
        change,
        currentDate
    };

    // 1. Permanently write and append to your log file storage
    // (Adjust "sales.txt" to match your system path if needed)
    appendSaleLog(receipt);

    // 2. Commit transaction to live RAM storage tracking
    salesLog.push_back(receipt);

    // 3. Wipe current checkout session cart clean for next client interaction
    cart.clear();

    return 'd'; 
}

std::string generateNextTransactionID(const std::vector<SaleReceipt>& logs) {
    // If there are no past transactions in the file, start with a default base ID
    if (logs.empty()) {
        return "TXN-1001";
    }

    // Grab the absolute last receipt in our vector memory
    std::string lastID = logs.back().transactionID; // e.g., "TXN-1002"

    // Verify it starts with our prefix, then parse the number out
    if (lastID.rfind("TXN-", 0) == 0) { 
        std::string numericPart = lastID.substr(4); // Extracts "1002"
        int nextNumber = std::stoi(numericPart) + 1; // 1003
        
        return "TXN-" + std::to_string(nextNumber); // Returns "TXN-1003"
    }

    return "TXN-1001"; // Fallback safety default
}

std::string getCurrentDate() {
    // Get the current system time point
    auto now = std::chrono::system_clock::now();
    
    // Convert to a time_t object
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    
    // Convert to local time structure
    std::tm* local_time = std::localtime(&current_time);
    
    // Format into YYYY-MM-DD
    std::stringstream ss;
    ss << std::put_time(local_time, "%Y-%m-%d");
    
    return ss.str();
}