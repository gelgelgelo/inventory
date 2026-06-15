#pragma once

#include "product.h"

void printInventoryTableSeparator();
void printInventoryTableHeader();
void printCartTableSeparator();
void printCartTableHeader();
void printProductInfo(const ProductInfo&);
void printStockInfo(const ProductInfo&);
void printCartInfo(const CartItem&);
void printReceiptHeader();
void printReceiptItem(const CartItem&);
void displayMainMenu();
void displayProdMngMenu();
void displayStockMonMenu();
void displayCustomerMenu();
void displayProducts(const std::vector<ProductInfo> &);
void displayStock(const std::vector<ProductInfo>&, bool = false);
void displayCart(const std::vector<CartItem>&, const double);
void displaySalesLog(const std::vector<SaleReceipt>& salesLog);


const int WIDTH_ID    = 6;
const int WIDTH_NAME  = 35;
const int WIDTH_PRICE = 8;
const int WIDTH_STOCK = 8;
const int WIDTH_T_PRICE = 14;
const int WIDTH_STOCK_AVAILABILITY = 16;
const int WIDTH_CUSTOMER_NAME = 20;
const int WIDTH_TXN_ID  = 10;

const int WIDTH_REC_DESC  = 22;
const int WIDTH_REC_QTY   = 6;
const int WIDTH_REC_PRICE = 11;
const int WIDTH_REC_TOTAL = 11;

const int CRIT_THRESHOLD = 5;