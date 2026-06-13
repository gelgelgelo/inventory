#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// STRUCTS AND GLOBALS

struct ProductInfo {
    std::string ID;
    std::string name;
    double price;
    int stockQnty;

};

struct SaleReceipt {
    std::string customerName;
    std::string productID;
    double productPrice;
    int qtyBought;
    double total;
    double customerPayment;
    double change;

};

struct ErrMsgs {
    const std::string missingArguments = "[Argument Error]: Arguments is incomplete.\n";
    const std::string fileCantBeRead = "[File Error]: File cannot be read.\n";
    const std::string corruptLineSkipped = "[File Error]: Line not parsed properly.\n";
    const std::string parsedWithSkippedLines = "[File Warning]: File read successfully, but some lines are skipped.\n\n";
    const std::string fileNotOpened = "[File Error]: File was not opened properly. Saving aborted.\n";
    const std::string savingFailed = "[System Error]: Failed to finalize the file save.\n";
    const std::string writingfailed = "[System Error]: There was an error writing into file.\n";
    const std::string invalidIntType  = "[Input Error]: Please enter a valid whole number.\n";
    const std::string invalidIntRange = "[Input Error]: Number is out of the allowed range.\n";
    const std::string invalidDoubleType  = "[Input Error]: Please enter a valid decimal number.\n";
    const std::string invalidDoubleRange = "[Input Error]: Value is out of the allowed range.\n";
    const std::string invalidStrLength = "[Input Error]: Text length does not meet the required limits.\n";
    const std::string invalidStrPipePresent = "[Input Error]: Pipe ('|') was detected. Please avoid this character\n";
    const std::string invalidCharType = "[Input Error]: Please enter only a single character.\n";
    const std::string invalidCharVal  = "[Input Error]: That choice is not recognized.\n";
    const std::string duplicateID = "[Input Error]: Product ID already exists. Please use a unique ID.\n";
    const std::string productNotFound = "[Search Error]: Product ID not found.\n";
}const errmsg;

struct FileStatus {
    static constexpr int SUCCESS = 0;
    static constexpr int WARNING_PARSING_ERRORS = 1;
    static constexpr int ERROR_FILE_NOT_READ = 2;
    static constexpr int ERROR_FILE_NOT_SAVED = 3;
    static constexpr int ERROR_FILE_NOT_OPENED = 4;
    static constexpr int ERROR_WRITING = 5;

}const fileStatus;


// file handling
int loadInventory(std::vector<ProductInfo>&, const std::string&);
ProductInfo parseLine(const std::string&);
int saveInventory(const std::vector<ProductInfo>&, const std::string&);
std::string encodeProductData(const ProductInfo&);

// inputs
std::string getString(const std::string& prompt = "", int min = 1, int max = 999999);
double getDouble(const std::string& prompt = "", double min = 0.0, double max = 999999.9);
int getInt(const std::string& prompt = "", int min = 0, int max = 999999);
char getChar(const std::string& prompt = "", char min = 'a', char max = 'z');
bool validateString(const std::string&);
bool validateDoubleStr(const std::string&);
bool validateIntStr(const std::string&);
bool isIdDuplicate(const std::string& id, const std::vector<ProductInfo>& inventory);

// display
void displayProducts(const std::vector<ProductInfo> &);
void updateProduct(std::vector<ProductInfo>&);
void addProduct(std::vector<ProductInfo>& inventory);


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
    updateProduct(productInventory);
    addProduct(productInventory);
    

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

 std::string getString(const std::string& prompt, int min, int max){
    std::string input;

    while(true){
        std::cout << prompt << "\n >> ";
        std::getline(std::cin, input);
        if((int)input.length() < min || (int)input.length() > max){
            std::cout << errmsg.invalidStrLength;
            continue;
        }
        if(!validateString(input)){
            std::cout << errmsg.invalidStrPipePresent;
            continue;
        }

        return input;
    }
} 

double getDouble(const std::string& prompt, double min, double max){
    std::string strInput;

    while(true){
        std::cout << prompt << "\n >> ";
        std::getline(std::cin, strInput);
        if(!validateDoubleStr(strInput)){
            std::cout << errmsg.invalidDoubleType;
            continue;
        }
        double dbInput = std::stod(strInput);
        if(dbInput < min || dbInput > max){
            std::cout << errmsg.invalidDoubleRange;
            continue;
        }
        
        return dbInput;
    }
}

int getInt(const std::string& prompt, int min, int max){
    std::string strInput;

    while(true){
        std::cout << prompt << "\n >> ";
        std::getline(std::cin, strInput);
        if(!validateIntStr(strInput)){
            std::cout << errmsg.invalidIntType;
            continue;
        }
        int intInput = std::stoi(strInput);
        if(intInput < min || intInput > max){
            std::cout << errmsg.invalidIntRange;
            continue;
        }
        
        return intInput;
    }
}

char getChar(const std::string& prompt, char min, char max){
    std::string strInput;

    while(true){
        std::cout << prompt << "\n >> ";
        std::getline(std::cin, strInput);
        if((int)strInput.length() != 1){
            std::cout << errmsg.invalidCharType;
            continue;
        }
        char chInput = strInput[0];
        if(chInput < min || chInput > max){
            std::cout << errmsg.invalidCharVal;
            continue;
        }

        return chInput;
    }
}

bool isIdDuplicate(const std::string& id, const std::vector<ProductInfo>& inventory) {
    for (const auto& product : inventory) {
        if (product.ID == id) {
            return true;
        }
    }
    return false;
}

bool validateString(const std::string& str){
    for(char c : str){
        if(c == '|') return false;
    }

    return true;
}

bool validateDoubleStr(const std::string& str){
    //immediately return false if empty input or single period
    if(str.empty() || str == ".") return false;
    
    bool periodFound = false;

    for(char c : str){
        if(c == '.'){
            if(!periodFound){
                periodFound = true;
                continue;
            }
            else return false;
        }
        if(c < '0' || c > '9') return false;
    }

    return true;
}

bool validateIntStr(const std::string& str){
    //immediately return false if empty input
    if(str.empty()) return false;
    for(char c : str){
        if(c < '0' || c > '9') return false;
    }

    return true;
}