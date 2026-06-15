#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>

#include "errMsg.h"
#include "fileHandling.h"
#include "product.h"
#include "inputHandling.h"


int loadInventory(std::vector<ProductInfo>& productInventory){
    std::string productRawData;
    ProductInfo processedData;
    bool fileClean = true;

    std::ifstream readFile(INVENTORY_FILE_PATH); // open file
    if(!readFile.is_open()) return fileStatus.ERROR_FILE_NOT_READ;// return if file not open

    while (std::getline (readFile, productRawData)) {
        if(productRawData.empty()) continue; // immediately skip empty lines

        processedData = parseInvLine(productRawData);

        if(processedData.ID == "[ERROR]"){ // the parser returns an [ERROR] id if it fails and line is skipped
            std::cout << errmsg.corruptLineSkipped;
            std::cout << " -> Raw Line : " << productRawData << "\n"; // user can check the suspected ccorrupt line
            fileClean = false;
            continue;
        }

        productInventory.push_back(processedData);
    }
    readFile.close(); // close the file

    if(!fileClean) return fileStatus.WARNING_PARSING_ERRORS;

    return fileStatus.SUCCESS;
}


ProductInfo parseInvLine(const std::string& productRawData){
    std::string ID, name;
    double price;
    int stockQnty;

    std::stringstream ss(productRawData);
    std::string token;

    std::getline(ss, token, '|');
    if(token.empty()){
        return {"[ERROR]", "", 0.0, 0};
    } else ID = token;

    std::getline(ss, token, '|');
    if(token.empty()){
        return {"[ERROR]", "", 0.0, 0};
    } else name = token;

    std::getline(ss, token, '|');
    if(!validateDoubleStr(token)){
        return {"[ERROR]", "", 0.0, 0};
    } else price = std::stod(token);

    std::getline(ss, token, '|');
    if(!validateIntStr(token)) {
        return {"[ERROR]", "", 0.0, 0};
    } else stockQnty = std::stoi(token);

    return {ID, name, price, stockQnty};
}

int saveInventory(const std::vector<ProductInfo>& productInventory){
    std::filesystem::path txtFilePath = INVENTORY_FILE_PATH;

    std::filesystem::path parentDir = txtFilePath.parent_path(); 
    std::filesystem::create_directories(parentDir);

    std::filesystem::path tempFilePath = txtFilePath;
    tempFilePath.replace_extension(".tmp");


    std::ofstream writeFile(tempFilePath);
    if(!writeFile.is_open()) return fileStatus.ERROR_FILE_NOT_OPENED;

    for(const ProductInfo& productInfo : productInventory){
        writeFile << encodeProductData(productInfo) << "\n";
    }
    // If the write failed, clean up the garbage file
    if (writeFile.bad()) {
        writeFile.close(); // ALWAYS close the stream before deleting!
        std::filesystem::remove(tempFilePath); 
        return fileStatus.ERROR_WRITING;
    } else writeFile.close();

    try {
        // 1. Drop the risky code inside the try block
        std::filesystem::rename(tempFilePath, txtFilePath);
        
        return fileStatus.SUCCESS;
    } 
    catch (const std::filesystem::filesystem_error& error) {
        std::cout << "\n[System Error]: Failed to finalize the file save.\n";
        std::cout << " -> Reason: " << error.what() << "\n";
        
        std::filesystem::remove(tempFilePath);
        
        return fileStatus.ERROR_FILE_NOT_SAVED;
    }
}

std::string encodeProductData(const ProductInfo& productData){
    std::string encodedData = "";
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << productData.price;
    std::string priceStr = ss.str();

    encodedData = productData.ID + "|" 
                + productData.name + "|" 
                + priceStr + "|"
                + std::to_string(productData.stockQnty);


    return encodedData;
}

int loadLogs(std::vector<SaleReceipt>& logs) {
    std::string rawLine;
    bool fileClean = true;

    std::ifstream readFile(SALES_FILE_PATH);
    if (!readFile.is_open()) return fileStatus.ERROR_FILE_NOT_READ;

    while (std::getline(readFile, rawLine)) {
        if (rawLine.empty()) continue;

        ParsedLineResult parsed = parseSaleLine(rawLine);

        if (!parsed.isValid) {
            std::cout << errmsg.corruptLineSkipped << " -> Raw Line : " << rawLine << "\n";
            fileClean = false;
            continue;
        }

        // If it belongs to the same transaction, append the item
        if (!logs.empty() && logs.back().transactionID == parsed.receiptInfo.transactionID) {
            logs.back().itemsBought.push_back(parsed.item);
        } 
        // Otherwise, create a new receipt entry
        else {
            SaleReceipt newReceipt = parsed.receiptInfo;
            newReceipt.itemsBought.push_back(parsed.item);
            logs.push_back(newReceipt);
        }
    }
    readFile.close();

    if (!fileClean) return fileStatus.WARNING_PARSING_ERRORS;
    return fileStatus.SUCCESS;
}


ParsedLineResult parseSaleLine(const std::string& rawLine) {
    ParsedLineResult result;
    std::stringstream ss(rawLine);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() != 10) {
        result.isValid = false;
        return result;
    }

    // Validate number strings
    if (!validateIntStr(tokens[4])    || !validateDoubleStr(tokens[5]) || 
        !validateDoubleStr(tokens[6]) || !validateDoubleStr(tokens[7]) || 
        !validateDoubleStr(tokens[8]) || !validateDoubleStr(tokens[9])) {
        result.isValid = false;
        return result;
    }

    // Populate Receipt Metadata
    result.receiptInfo.transactionID = tokens[0];
    result.receiptInfo.customerName = tokens[1];
    result.receiptInfo.date = tokens[2];
    result.receiptInfo.grandTotal = std::stod(tokens[7]);
    result.receiptInfo.customerPayment = std::stod(tokens[8]);
    result.receiptInfo.change = std::stod(tokens[9]);

    // Populate Cart Item snapshot directly
    result.item.ID = tokens[3];
    result.item.name = ""; // Leave blank or map if needed, but ID and Price are captured!
    result.item.orderQty = std::stoi(tokens[4]);
    result.item.price = std::stod(tokens[5]);
    result.item.total = std::stod(tokens[6]);

    return result;
}


int appendSaleLog(const SaleReceipt& receipt) {
    std::ofstream writeFile(SALES_FILE_PATH, std::ios::app);
    if (!writeFile.is_open()) return fileStatus.ERROR_FILE_NOT_OPENED;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);

    for (const auto& item : receipt.itemsBought) {
        ss.str(""); 
        ss.clear();
        
        ss << receipt.transactionID << "|"
           << receipt.customerName << "|"
           << receipt.date << "|"
           << item.ID << "|"
           << item.orderQty << "|"
           << item.price << "|"
           << item.total << "|"
           << receipt.grandTotal << "|"
           << receipt.customerPayment << "|"
           << receipt.change;

        writeFile << ss.str() << "\n";
    }

    if (writeFile.bad()) {
        writeFile.close();
        return fileStatus.ERROR_WRITING;
    }

    writeFile.close();
    return fileStatus.SUCCESS;
}