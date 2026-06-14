#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>

#include "fileHandling.h"
#include "product.h"
#include "errMsg.h"
#include "inputHandling.h"

int loadInventory(std::vector<ProductInfo>& productInventory, const std::string& filePath){
    std::string productRawData;
    ProductInfo processedData;
    bool fileClean = true;

    std::ifstream readFile(filePath); // open file
    if(!readFile.is_open()) return fileStatus.ERROR_FILE_NOT_READ;// return if file not open

    while (std::getline (readFile, productRawData)) {
        if(productRawData.empty()) continue; // immediately skip empty lines

        processedData = parseLine(productRawData);

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

ProductInfo parseLine(const std::string& productRawData){
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

int saveInventory(const std::vector<ProductInfo>& productInventory, const std::string& filePath){
    std::filesystem::path txtFilePath = filePath;

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