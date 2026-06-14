#pragma once

#include <vector>
#include "product.h" // Needed for ProductInfo

// Only expose the main entry point
void processOrder(std::vector<ProductInfo>& products);