#pragma once
#include <string>
#include <vector>

#include "product.h"

void updateProduct(std::vector<ProductInfo>&);
void addProduct(std::vector<ProductInfo>& inventory);
void searchProduct(const std::vector<ProductInfo>& products);
void processOrder(std::vector<ProductInfo>& products);
void deleteProduct(std::vector<ProductInfo>& products);