#pragma once

#include "product.h"

void printInventoryTableSeparator();
void printInventoryTableHeader();
void printCartTableSeparator();
void printCartTableHeader();
void printProductInfo(const ProductInfo&);
void printCartInfo(const CartItem&);
void displayMainMenu();
void displayProdMngMenu();
void displayStockMonMenu();
void displayCustomerMenu();
void displayProducts(const std::vector<ProductInfo> &);
void displayCart(const std::vector<CartItem>&, const double);


const int WIDTH_ID    = 6;
const int WIDTH_NAME  = 35;
const int WIDTH_PRICE = 8;
const int WIDTH_STOCK = 8;
const int WIDTH_T_PRICE = 14;
const int WIDTH_STOCK_AVAILABILITY = 8;