#pragma once

#include "productInfo.h"

void printInventoryTableSeparator();
void printInventoryTableHeader();
void printProductInfo(const ProductInfo&);

const int WIDTH_ID    = 6;
const int WIDTH_NAME  = 35;
const int WIDTH_PRICE = 8;
const int WIDTH_STOCK = 8;
const int WIDTH_STOCK_AVAILABILITY = 8;