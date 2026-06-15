#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// header files
#include "errMsg.h"
#include "inventoryManagement.h"
#include "orderSystem.h"
#include "product.h"
#include "inputHandling.h"
#include "display.h"
#include "fileHandling.h"

void productManagement(std::vector<ProductInfo>&);
void stockMonitoring(const std::vector<ProductInfo>&);
void customerOrders(std::vector<ProductInfo>&, std::vector<SaleReceipt>&);
bool isSure();

int main()
{
    // initialize inventory vector
    std::vector<ProductInfo> productInventory;
    std::vector<SaleReceipt> salesLog;

    // check if the file was read properly

    int status = loadInventory(productInventory);
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

    status = loadLogs(salesLog);
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


    std::vector<ProductInfo> tempVec = productInventory;
    bool isRunning = true;
    int choice = 0;

    while(isRunning){
        
        displayMainMenu();
        
        choice = getInt("", 0, 4);

        switch(choice){
            case 1 :
                productManagement(productInventory);
                break;
            case 2 :
                stockMonitoring(productInventory);
                break;
            case 3 :
                customerOrders(productInventory, salesLog);
                break;
            case 4 : // holy nesting
                status = saveInventory(productInventory);
                switch(status){
                    case fileStatus.SUCCESS : 
                        std::cout << "File updated successfully.\n\n";
                        tempVec = productInventory;
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
                break;
            case 0 :
                if(tempVec == productInventory || isSure()){
                    std::cout << "Exiting Program...\n\n";
                    isRunning = false;
                } 
                break;
        }

    }

    return 0;
}

void productManagement(std::vector<ProductInfo>& productInventory){
    bool isRunning = true;
    int choice = 0;
    while(isRunning){
        displayProdMngMenu();

        choice = getInt("", 0, 5);

        switch(choice){
            case 1 :
                displayProducts(productInventory);
                break;
            case 2 :
                searchProduct(productInventory);
                break;
            case 3 :   
                updateProduct(productInventory);
                break;
            case 4 :
                addProduct(productInventory);
                break;
            case 5 :   
                deleteProduct(productInventory);
                break;
            case 0 : 
                isRunning = false;
                std::cout << "Exiting Product Management... \n\n";
                break;
        }
    }
}

void stockMonitoring(const std::vector<ProductInfo>& productInventory){
    bool isRunning = true;
    int choice = 0;
    while(isRunning){
        displayStockMonMenu();

        choice = getInt("", 0, 5);

        switch(choice){
            case 1 :
                displayStock(productInventory);
                break;
            case 2 :
                displayStock(productInventory, true);
                break;
            case 0 : 
                isRunning = false;
                std::cout << "Exiting Stock Monitoring... \n\n";
                break;
        }
    }
}

void customerOrders(std::vector<ProductInfo>& productInventory, std::vector<SaleReceipt>& salesLog){
    bool isRunning = true;
    int choice = 0;
    while(isRunning){
        displayCustomerMenu();

        choice = getInt("", 0, 5);

        switch(choice){
            case 1 :
                processOrder(productInventory, salesLog);
                break;
            case 2 :
                displaySalesLog(salesLog);
                break;
            case 0 : 
                isRunning = false;
                std::cout << "Exiting Customer Orders... \n\n";
                break;
        }
    }
}

bool isSure(){
    char choice = getChar("[WARNING] Changes not saved. Exit anyways? ('Y'/'n')", 'A','z');
    return (choice == 'Y' || choice == 'y');
}