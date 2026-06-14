#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

#include "errMsg.h"
#include "inputHandling.h"
#include "product.h"
#include "display.h"

void addProduct(std::vector<ProductInfo>& inventory)
{
    std::string ID;
    std::string name;
    double price;
    int stockQnty;
    
    while (true) {
        ID = getString("Enter Product ID ('cancel' to cancel) : ", 4, 6);
        if(ID == "cancel")
            return;

        if (isIdDuplicate(ID, inventory)) {
            std::cout << errmsg.duplicateID;
            continue;
        }
        
        break;
    }
    name = getString("Enter Product Name ('cancel' to cancel) :", 1, 100);
    if(name == "cancel")
        return;
    price = getDouble("Enter Product Price (0.0 to cancel)", 0.0);
    if(price == 0.0)
        return;
    stockQnty = getInt("Enter Stock Quantity (0 to cancel)", 0);
    if(stockQnty == 0)
        return;

    inventory.push_back({ID, name, price, stockQnty});

    std::cout << "\nProduct added successfully!\n";
}




void updateProduct(std::vector<ProductInfo>& products)
{
    std::string targetID;
    ProductInfo* targetProduct = nullptr;
    bool found = false;
    char choice;

    targetID = getString("\nEnter Product ID to update ('cancel' to cancel): ", 4, 6);
    if(targetID == "cancel")
        return;

    // Looking for product
    for (auto &p : products)
    {
        if (p.ID == targetID)
        {
            found = true; 
            targetProduct = &p;
            break;
        }
    }

    if (!found){
        std::cout << errmsg.productNotFound;
        return;
    }
        
            
    do {
        std::cout << "\n--- UPDATE MENU ---\n";
        std::cout << "Current Product: " << targetProduct->name << " (ID: " << targetProduct->ID << ")\n";
        std::cout << "a. Name (Current: " << targetProduct->name << ")\n";
        std::cout << "b. Price (Current: " << targetProduct->price << ")\n";
        std::cout << "c. Stock Quantity (Current: " << targetProduct->stockQnty << ")\n";
        std::cout << "d. Exit\n";
        
        choice = getChar("Select an option (a-d):", 'a', 'd');

        switch(choice) {
            case 'a':
                targetProduct->name = getString("Enter New Name : ", 1, 50);
                std::cout << "Name updated!\n";
                break;
            case 'b':
                targetProduct->price = getDouble("Enter New Price : ", 0.01);
                std::cout << "Price updated!\n";
                break;
            case 'c':
                targetProduct->stockQnty = getInt("Enter New Stock Quantity : ", 1);
                std::cout << "Stock updated!\n";
                break;
            case 'd':
                std::cout << "Exiting update menu...\n";
                break;
        }
    } while (choice != 'd');

    std::cout << "\nProduct updated successfully!\n";
    return;
}

void searchProduct(const std::vector<ProductInfo>& products) {
    if (products.empty()) {
        std::cout << errmsg.searchUnavailable;
        return;
    }

    char choice;
    
    while (choice != 'c') {
        std::cout << "\n=== PRODUCT SEARCH CENTER ===\n";
        std::cout << "[a] Search by Product ID Patterns (e.g. 'e40')\n";
        std::cout << "[b] Search by Product Name Keyword\n";
        std::cout << "[c] Exit Search Mode Menu\n";
        choice = getChar("", 'a', 'c');

        if (choice == 'a') {
            std::string idPattern = getString("Enter structural ID Patterns:", 4);
            
            std::string lowerPattern = idPattern;
            std::transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);
            
            bool found = false;

            std::cout << "\n";

            for (const auto& p : products) {
                std::string lowerID = p.ID;
                std::transform(lowerID.begin(), lowerID.end(), lowerID.begin(), ::tolower);

                if (lowerID.find(lowerPattern) != std::string::npos) {
                    found = true;
                    printProductInfo(p);
                }
            }
            if (!found) std::cout << errmsg.productNotFound;

        } else if (choice == 'b') {
            std::string keyword = getString("Enter keywords or names:", 1, 50);
            
            std::string lowerKeyword = keyword;
            std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
            
            bool found = false;

            for (const auto& p : products) {
                std::string lowerName = p.name;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

                if (lowerName.find(lowerKeyword) != std::string::npos) {
                    found = true;
                    printProductInfo(p);
                }
            }
            if (!found) std::cout << errmsg.nameNotFound;
        }
    }
    std::cout << "\nExiting search system modules.\n";
}

void deleteProduct(std::vector<ProductInfo>& products) {
    std::cout << "\n--- Delete Product Record ---\n";

    if (products.empty()) {
        std::cout << "Error: Inventory empty.\n";
        return;
    }

    std::string targetID = getString("\nEnter Product ID to delete ('cancel' to cancel): ", 4, 6);
    if(targetID == "cancel")
        return;

    bool found = false;
    int id = 0;

    for (const ProductInfo &p : products) {
        if (p.ID == targetID) { 
            found = true;
            std::cout << "\n[Product Found]\n";
            break;
        }
        id++;
    }

    if (!found) {
        std::cout << errmsg.productNotFound;
        return;
    }

    printProductInfo(products[id]);
    
    char confirm = getChar("Confirm deletion? (y/n): ", 'A', 'z');
    
    if (std::tolower(confirm) == 'y') {
        products.erase(products.begin() + id); 
        std::cout << "Product dropped.\n";
    } else {
        std::cout << "Cancelled.\n";
    }
}