#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// header files
#include "errMsg.h"
#include "fileStatus.h"
#include "productInfo.h"
//#include "saleReceipt.h" // comment this out when we need to use this
#include "inputHandling.h"


// file handling
int loadInventory(std::vector<ProductInfo>&, const std::string&);
ProductInfo parseLine(const std::string&);
int saveInventory(const std::vector<ProductInfo>&, const std::string&);
std::string encodeProductData(const ProductInfo&);

// inputs


// display
void displayProducts(const std::vector<ProductInfo> &);
void updateProduct(std::vector<ProductInfo>&);
void addProduct(std::vector<ProductInfo>& inventory);
void searchProduct(const std::vector<ProductInfo>& products);

int main()
{
    // initialize inventory vector
    std::vector<ProductInfo> productInventory;

    // check if the file was read properly
    std::string filePath = "inv/inventory.txt";
    int status = loadInventory(productInventory, filePath);

    
    std::cout << "\n\n";
    switch(status){
        case fileStatus.SUCCESS : 
            std::cout << "File read successfully.\n\n";
            break;
        
        case fileStatus.WARNING_PARSING_ERRORS :
            std::cout << errmsg.parsedWithSkippedLines;
            break;

        case fileStatus.ERROR_FILE_NOT_READ :
            std::cout << errmsg.fileCantBeRead;
            return 1;
    }

    
    displayProducts(productInventory);
    searchProduct(productInventory);

    status = saveInventory(productInventory, filePath);
    switch(status){
        case fileStatus.SUCCESS : 
            std::cout << "File updated successfully.\n\n";
            break;
        
        case fileStatus.ERROR_FILE_NOT_OPENED :
            std::cout << errmsg.fileNotOpened;
            break;

        case fileStatus.ERROR_WRITING :
            std::cout << errmsg.writingfailed;
            break;
        case fileStatus.ERROR_FILE_NOT_SAVED :
         std::cout << errmsg.savingFailed;
         break;
    }

        

    return 0;
}

void displayProducts(const std::vector<ProductInfo> &products)
{
    for(const auto &p : products) {
        if(p.stockQnty != 0) {
            std::cout	<< "ID: " << p.ID << " | "
                        << "name: " << p.name << " | "
                        << "price: " << p.price << " | "
                        << "stock: " << p.stockQnty << "\n";
        }
    }	
}

void addProduct(std::vector<ProductInfo>& inventory)
{
    std::string ID;
    std::string name;
    double price;
    int stockQnty;
    
    while (true) {
        ID = getString("Enter Product ID", 4);
        
        if (isIdDuplicate(ID, inventory)) {
            std::cout << errmsg.duplicateID;
            continue;
        }
        
        break;
    }
    name = getString("Enter Product Name", 1, 100);
    price = getDouble("Enter Product Price", 0.01);
    stockQnty = getInt("Enter Stock Quantity", 0);

    inventory.push_back({ID, name, price, stockQnty});

    std::cout << "\nProduct added successfully!\n";
}


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

void updateProduct(std::vector<ProductInfo>& products)
{
    std::string targetID;
    ProductInfo* targetProduct = nullptr;
    bool found = false;
    char choice;

    targetID = getString("\nEnter Product ID to update: ", 4);

    // Looking for product
    for (auto &p : products)
    {
        if (p.ID == targetID)
        {
            found = true; 
            targetProduct = &p;
            break;
        }
    }

    if (!found){
        std::cout << errmsg.productNotFound;
        return;
    }
        
            
    do {
        std::cout << "\n--- UPDATE MENU ---\n";
        std::cout << "Current Product: " << (*targetProduct).name << " (ID: " << (*targetProduct).ID << ")\n";
        std::cout << "a. Name (Current: " << (*targetProduct).name << ")\n";
        std::cout << "b. Price (Current: " << (*targetProduct).price << ")\n";
        std::cout << "c. Stock Quantity (Current: " << (*targetProduct).stockQnty << ")\n";
        std::cout << "d. Exit\n";
        
        choice = getChar("Select an option (a-d):", 'a', 'd');

        switch(choice) {
            case 'a':
                (*targetProduct).name = getString("Enter New Name: ", 1, 50);
                std::cout << "Name updated!\n";
                break;
            case 'b':
                (*targetProduct).price = getDouble("Enter New Price: ", 0.01);
                std::cout << "Price updated!\n";
                break;
            case 'c':
                (*targetProduct).stockQnty = getInt("Enter New Stock Quantity: ", 1);
                std::cout << "Stock updated!\n";
                break;
            case 'd':
                std::cout << "Exiting update menu...\n";
                break;
        }
    } while (choice != 'd');

    std::cout << "\nProduct updated successfully!\n";
    return;
}

void searchProduct(const std::vector<ProductInfo>& products) {
    if (products.empty()) {
        std::cout << errmsg.searchUnavailable;
        return;
    }

    int choice = 0;

    while (choice != 3) {
        std::cout << "\n=== PRODUCT QUERY SEARCH CENTER ===\n";
        std::cout << "[1] Search by Product ID Structural Pattern (e.g. 'E40')\n";
        std::cout << "[2] Search by Product Name Keyword\n";
        std::cout << "[3] Exit Search Mode Menu\n";
        choice = getInt("Select entry point parameter (1-3):", 1, 3);



        if (choice == 1) {
            std::string idPattern = getString("Enter ID patterns:", 1, 20);
            bool found = false;

            for (const auto& p : products) {
                if (p.ID.find(idPattern) != std::string::npos) {
                    if (!found) {
                    found = true;
                    std::cout << "-------------------------------------------------------------\n"
                              << "ID: " << p.ID 
                              << " | name: " << p.name 
                              << " | price: " << p.price 
                              << " | stock: " << p.stockQnty << "\n";
                    }
                }
            }
            if (!found) std::cout << errmsg.productNotFound;
            else std::cout << "-------------------------------------------------------------\n";

        } else if (choice == 2) {
            std::string keyword = getString("Enter keywords or names:", 1, 30);
            bool found = false;

            for (const auto& p : products) {
                if (p.name.find(keyword) != std::string::npos) {
                    if (!found) {
                    found = true;
                    std::cout <<  "-------------------------------------------------------------\n"
                              << "ID: " << p.ID 
                              << " | name: " << p.name 
                              << " | price: " << p.price 
                              << " | stock: " << p.stockQnty << "\n";
                    }
                }
            }
            if (!found) std::cout << errmsg.nameNotFound;
            else std::cout << "-------------------------------------------------------------\n";
        }
    }
    std::cout << "Exiting search system modules.\n";
}