#include <iostream>
#include <iomanip>
#include <vector>


#include "display.h"
#include "product.h"

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