#include <iostream>
#include <iomanip>
#include <vector>


#include "display.h"
#include "product.h"

void displayMainMenu(){
    std::string mainMenu = "\n\n===== PRODUCT INVENTORY SYSTEM =====\n\n"
                           "      [1] Product Management\n"
                           "      [2] Stock Monitoring\n"
                           "      [3] Customer Orders\n"
                           "      [4] Save Inventory\n"
                           "      [0] Exit";
    std::cout << mainMenu << std::endl;
}

void displayProdMngMenu(){
    std::string prodMngMenu = "\n\n--- PRODUCT MANAGEMENT ---\n\n"
                              "    [1] Display\n"
                              "    [2] Search\n"
                              "    [3] Update\n"
                              "    [4] Add\n"
                              "    [5] Delete\n"
                              "    [0] Exit";

    std::cout << prodMngMenu << std::endl;
}

void displayStockMonMenu(){
    std::string stockMonMenu = "\n\n--- STOCK MONITORING ---\n\n"
                               "    [1] Display All\n"
                               "    [2] Display Critical\n"
                               "    [0] Exit";

    std::cout << stockMonMenu << std::endl;
}

void displayCustomerMenu(){
    std::string customerMenu = "\n\n--- CUSTOMER ORDERS ---\n\n"
                               "    [1] Process New Order\n"
                               "    [2] View Sales Log\n"
                               "    [0] Exit";

    std::cout << customerMenu << std::endl;
}


void printInventoryTableSeparator() {
    // Total width is the sum of columns + space for dividers
    int totalWidth = WIDTH_ID + WIDTH_NAME + WIDTH_PRICE + WIDTH_STOCK + 7; 
    std::cout << " " << std::string(totalWidth, '-') << "\n";
}

void printInventoryTableHeader() {
    printInventoryTableSeparator();
    std::cout << "| " << std::left  << std::setw(WIDTH_ID)    << "ID"
              << "| " << std::left  << std::setw(WIDTH_NAME)  << "Product Name"
              << "| " << std::right << std::setw(WIDTH_PRICE) << "Price"
              << "| " << std::right << std::setw(WIDTH_STOCK) << "Stock" << "|\n";
    printInventoryTableSeparator();
}

void printProductInfo(const ProductInfo& p){
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "| " << std::left  << std::setw(WIDTH_ID)    << p.ID
              << "| " << std::left  << std::setw(WIDTH_NAME)  << p.name
              << "| " << std::right << std::setw(WIDTH_PRICE) << p.price
              << "| " << std::right << std::setw(WIDTH_STOCK) << p.stockQnty << "|\n";
}

void printCartTableSeparator(){
    int totalWidth = WIDTH_NAME + WIDTH_PRICE + WIDTH_STOCK + WIDTH_T_PRICE + 7; 
    std::cout << " " << std::string(totalWidth, '-') << "\n";
}

void printCartTableHeader(){
    printCartTableSeparator();
    std::cout << "| " << std::left  << std::setw(WIDTH_NAME)  << "Cart"
              << "| " << std::right << std::setw(WIDTH_PRICE) << "Price"
              << "| " << std::right << std::setw(WIDTH_STOCK) << "Order"
              << "| " << std::right << std::setw(WIDTH_T_PRICE) << "Total" << "|\n";
    printCartTableSeparator();
}

void printCartInfo(const CartItem& ci){
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "| " << std::left  << std::setw(WIDTH_NAME)  << ci.product->name
              << "| " << std::right << std::setw(WIDTH_PRICE) << ci.product->price
              << "| " << std::right << std::setw(WIDTH_STOCK) << ci.orderQty
              << "| " << std::right << std::setw(WIDTH_T_PRICE) << ci.orderQty*ci.product->price << "|\n";
}


void displayProducts(const std::vector<ProductInfo> &products)
{

    printInventoryTableHeader();

    for(const auto &p : products) {
        if(p.stockQnty != 0) {
            printProductInfo(p);
        }
    }	

    printInventoryTableSeparator();
}

void displayCart(const std::vector<CartItem>& cart, const double total){
    printCartTableHeader();
    for(const CartItem& ci : cart){    
        printCartInfo(ci);
    }
    printCartTableSeparator();

    std::cout << "Total : " << total << "\n\n";
}