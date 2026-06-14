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
#include "product.h"
#include "inputHandling.h"
#include "display.h"
#include "fileHandling.h"


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
    addProduct(productInventory);
    updateProduct(productInventory);

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

