#pragma once

#include <string>
#include <vector>

#include "product.h"

const std::string INVENTORY_FILE_PATH = "inv/inventory.txt";
const std::string SALES_FILE_PATH = "inv/salesLog.txt";


struct ParsedLineResult {
    SaleReceipt receiptInfo;
    CartItem item;
    bool isValid = true;
};

int loadInventory(std::vector<ProductInfo>&);
ProductInfo parseInvLine(const std::string&);
int saveInventory(const std::vector<ProductInfo>&);
std::string encodeProductData(const ProductInfo&);
int loadLogs(std::vector<SaleReceipt>&);
ParsedLineResult parseSaleLine(const std::string&);
int appendSaleLog(const SaleReceipt&);


struct FileStatus {
    static constexpr int SUCCESS = 0;
    static constexpr int WARNING_PARSING_ERRORS = 1;
    static constexpr int ERROR_FILE_NOT_READ = 2;
    static constexpr int ERROR_FILE_NOT_SAVED = 3;
    static constexpr int ERROR_FILE_NOT_OPENED = 4;
    static constexpr int ERROR_WRITING = 5;

}const fileStatus;