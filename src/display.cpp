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

void displayProducts(const std::vector<ProductInfo> &products) {
    printInventoryTableHeader();
    for(const auto &p : products) {
        if(p.stockQnty != 0) {
            printProductInfo(p);
        }
    }   
    printInventoryTableSeparator();
}

void printStockInfo(const ProductInfo& p){
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "| " << std::left  << std::setw(WIDTH_ID)    << p.ID
              << "| " << std::left  << std::setw(WIDTH_NAME)  << p.name
              << "| " << std::right << std::setw(WIDTH_STOCK) << p.stockQnty
              << "| " << std::right << std::setw(WIDTH_STOCK_AVAILABILITY) << p.availability() << "|\n";
}

void printStockTableSeparator() {
    int totalWidth = WIDTH_ID + WIDTH_NAME + WIDTH_STOCK + WIDTH_STOCK_AVAILABILITY + 7; 
    std::cout << " " << std::string(totalWidth, '-') << "\n";
}

void printStockTableHeader() {
    printStockTableSeparator();
    std::cout << "| " << std::left  << std::setw(WIDTH_ID)    << "ID"
              << "| " << std::left  << std::setw(WIDTH_NAME)  << "Product Name"
              << "| " << std::right << std::setw(WIDTH_STOCK) << "Qty"
              << "| " << std::right << std::setw(WIDTH_STOCK_AVAILABILITY) << "Status" << "|\n";
    printStockTableSeparator();
}

void displayStock(const std::vector<ProductInfo>& products, bool critOnly){

    printStockTableHeader();
    for(const auto &p : products) {
        if(!critOnly || p.stockQnty <= CRIT_THRESHOLD) {
            printStockInfo(p);
        }
    }   
    printStockTableSeparator();
}
// ==========================================
// CART TABLE PRINTERS
// ==========================================

void printCartTableSeparator(){
    int totalWidth = WIDTH_NAME + WIDTH_PRICE + WIDTH_STOCK + WIDTH_T_PRICE + 7; 
    std::cout << " " << std::string(totalWidth, '-') << "\n";
}

void printCartTableHeader(){
    printCartTableSeparator();
    std::cout << "| " << std::left  << std::setw(WIDTH_NAME)    << "Cart"
              << "| " << std::right << std::setw(WIDTH_PRICE)   << "Price"
              << "| " << std::right << std::setw(WIDTH_STOCK)   << "Order"
              << "| " << std::right << std::setw(WIDTH_T_PRICE) << "Total" << "|\n";
    printCartTableSeparator();
}

void printCartInfo(const CartItem& ci){
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "| " << std::left  << std::setw(WIDTH_NAME)    << ci.name
              << "| " << std::right << std::setw(WIDTH_PRICE)   << ci.price
              << "| " << std::right << std::setw(WIDTH_STOCK)   << ci.orderQty
              << "| " << std::right << std::setw(WIDTH_T_PRICE) << ci.total << "|\n";
}

void displayCart(const std::vector<CartItem>& cart, const double total){
    printCartTableHeader();
    for(const CartItem& ci : cart){    
        printCartInfo(ci);
    }
    printCartTableSeparator();
    std::cout << "Total : " << total << "\n\n";
}

// ==========================================
// DYNAMIC RECEIPT PRINTERS
// ==========================================

void printReceiptSeparator(char ch = '=') {
    int totalWidth = WIDTH_REC_DESC + WIDTH_REC_QTY + WIDTH_REC_PRICE + WIDTH_REC_TOTAL;
    std::cout << std::string(totalWidth, ch) << "\n";
}

void printReceiptHeader(){
    int totalWidth = WIDTH_REC_DESC + WIDTH_REC_QTY + WIDTH_REC_PRICE + WIDTH_REC_TOTAL;
    std::string title = "TRANSACTION RECEIPT";
    
    std::cout << "\n";
    printReceiptSeparator('=');
    
    // Dynamic text centering based on column widths
    if (totalWidth > static_cast<int>(title.length())) {
        int padding = (totalWidth - title.length()) / 2;
        std::cout << std::string(padding, ' ') << title << "\n";
    } else {
        std::cout << title << "\n";
    }
    
    printReceiptSeparator('=');
    std::cout << std::left  << std::setw(WIDTH_REC_DESC)  << "Item Description" 
              << std::right << std::setw(WIDTH_REC_QTY)   << "Qty" 
              <<               std::setw(WIDTH_REC_PRICE) << "Unit Price" 
              <<               std::setw(WIDTH_REC_TOTAL) << "Total" << "\n";
    printReceiptSeparator('=');
}

void printReceiptItem(const CartItem& ci){
    std::cout << std::fixed << std::setprecision(2);
    std::string displayName = ci.name;

    // Dynamic truncation threshold based on the actual description column width
    int maxAllowedLength = WIDTH_REC_DESC - 1; 
    if (static_cast<int>(displayName.length()) > maxAllowedLength) {
        displayName = displayName.substr(0, WIDTH_REC_DESC - 4) + "...";
    }

    std::cout << std::left  << std::setw(WIDTH_REC_DESC)  << displayName
              << std::right << std::setw(WIDTH_REC_QTY)   << ci.orderQty
              <<               std::setw(WIDTH_REC_PRICE) << ci.price
              <<               std::setw(WIDTH_REC_TOTAL) << ci.total << "\n";
}

// ==========================================
// DYNAMIC SALES LOG PRINTERS
// ==========================================

void printSalesLogSeparator(char ch = '=') {
    int totalWidth = WIDTH_TXN_ID + WIDTH_CUSTOMER_NAME + WIDTH_ID + WIDTH_STOCK + WIDTH_T_PRICE + 1;
    std::cout << std::string(totalWidth, ch) << "\n";
}

void displaySalesLog(const std::vector<SaleReceipt>& salesLog) {
    if (salesLog.empty()) {
        std::cout << "\n[Notice] No sales logs available to display.\n";
        return;
    }

    std::cout << "\n";
    printSalesLogSeparator('=');
    std::cout << std::left  << std::setw(WIDTH_TXN_ID)       << "ID"
              <<            std::setw(WIDTH_CUSTOMER_NAME) << "Name"
              <<            std::setw(WIDTH_ID)            << "Item ID"
              << std::right << std::setw(WIDTH_STOCK)         << "Qty"
              <<            std::setw(WIDTH_T_PRICE)       << "Total" << "\n";
    printSalesLogSeparator('=');

    for (const auto& receipt : salesLog) {
        bool isFirstItem = true;

        for (const auto& item : receipt.itemsBought) {
            if (isFirstItem) {
                std::cout << std::left << std::setw(WIDTH_TXN_ID) << receipt.transactionID
                          << std::setw(WIDTH_CUSTOMER_NAME) << receipt.customerName;
                isFirstItem = false;
            } else {
                std::cout << std::left << std::setw(WIDTH_TXN_ID) << ""
                          << std::setw(WIDTH_CUSTOMER_NAME) << "";
            }

            std::cout << std::left  << std::setw(WIDTH_ID)      << item.ID
                      << std::right << std::setw(WIDTH_STOCK)   << item.orderQty
                      <<               std::setw(WIDTH_T_PRICE) << std::fixed << std::setprecision(2) << item.total 
                      << "\n";
        }
        printSalesLogSeparator('-');
    }
}