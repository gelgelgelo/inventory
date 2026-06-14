#include <iostream>
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
        ID = getString("Enter Product ID", 4);
        
        if (isIdDuplicate(ID, inventory)) {
            std::cout << errmsg.duplicateID;
            continue;
        }
        
        break;
    }
    name = getString("Enter Product Name", 1, 100);
    price = getDouble("Enter Product Price", 0.01);
    stockQnty = getInt("Enter Stock Quantity", 0);

    inventory.push_back({ID, name, price, stockQnty});

    std::cout << "\nProduct added successfully!\n";
}




void updateProduct(std::vector<ProductInfo>& products)
{
    std::string targetID;
    ProductInfo* targetProduct = nullptr;
    bool found = false;
    char choice;

    targetID = getString("\nEnter Product ID to update: ", 4);

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
        std::cout << "Current Product: " << (*targetProduct).name << " (ID: " << (*targetProduct).ID << ")\n";
        std::cout << "a. Name (Current: " << (*targetProduct).name << ")\n";
        std::cout << "b. Price (Current: " << (*targetProduct).price << ")\n";
        std::cout << "c. Stock Quantity (Current: " << (*targetProduct).stockQnty << ")\n";
        std::cout << "d. Exit\n";
        
        choice = getChar("Select an option (a-d):", 'a', 'd');

        switch(choice) {
            case 'a':
                (*targetProduct).name = getString("Enter New Name: ", 1, 50);
                std::cout << "Name updated!\n";
                break;
            case 'b':
                (*targetProduct).price = getDouble("Enter New Price: ", 0.01);
                std::cout << "Price updated!\n";
                break;
            case 'c':
                (*targetProduct).stockQnty = getInt("Enter New Stock Quantity: ", 1);
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

    int choice = 0;
    
    while (choice != 3) {
        std::cout << "\n=== PRODUCT SEARCH CENTER ===\n";
        std::cout << "[1] Search by Product ID Patterns (e.g. 'e40')\n";
        std::cout << "[2] Search by Product Name Keyword\n";
        std::cout << "[3] Exit Search Mode Menu\n";
        choice = getInt("Select entry point parameter (1-3):", 1, 3);

        if (choice == 1) {
            std::string idPattern = getString("Enter structural ID Patterns:", 1, 20);
            
            std::string lowerPattern = idPattern;
            std::transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);
            
            bool found = false;

            for (const auto& p : products) {
                std::string lowerID = p.ID;
                std::transform(lowerID.begin(), lowerID.end(), lowerID.begin(), ::tolower);

                if (lowerID.find(lowerPattern) != std::string::npos) {
                    found = true;
                    printProductInfo(p);
                }
            }
            if (!found) std::cout << errmsg.productNotFound;

        } else if (choice == 2) {
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
    std::cout << "Exiting search system modules.\n";
}