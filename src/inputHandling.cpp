#include <iostream>
#include <string>
#include <vector>

#include "errMsg.h"
#include "product.h"

std::string getString(const std::string& prompt = "", int min = 1, int max = 999999);
double getDouble(const std::string& prompt = "", double min = 0.0, double max = 999999.9);
int getInt(const std::string& prompt = "", int min = 0, int max = 999999);
char getChar(const std::string& prompt = "", char min = 'a', char max = 'z');
bool validateString(const std::string&);
bool validateDoubleStr(const std::string&);
bool validateIntStr(const std::string&);
bool isIdDuplicate(const std::string& id, const std::vector<ProductInfo>& inventory);

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