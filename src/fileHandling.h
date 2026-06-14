#pragma once

#include <string>
#include <vector>

#include "product.h"

int loadInventory(std::vector<ProductInfo>&, const std::string&);
ProductInfo parseLine(const std::string&);
int saveInventory(const std::vector<ProductInfo>&, const std::string&);
std::string encodeProductData(const ProductInfo&);

struct FileStatus {
    static constexpr int SUCCESS = 0;
    static constexpr int WARNING_PARSING_ERRORS = 1;
    static constexpr int ERROR_FILE_NOT_READ = 2;
    static constexpr int ERROR_FILE_NOT_SAVED = 3;
    static constexpr int ERROR_FILE_NOT_OPENED = 4;
    static constexpr int ERROR_WRITING = 5;

}const fileStatus;