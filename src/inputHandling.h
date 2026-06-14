#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "errMsg.h"
#include "product.h"

std::string getString(const std::string& prompt = "", int min = 1, int max = 999999);
double getDouble(const std::string& prompt = "", double min = 0.0, double max = 999999.9);
int getInt(const std::string& prompt = "", int min = 0, int max = 999999);
char getChar(const std::string& prompt = "", char min = 'A', char max = 'z');
bool validateString(const std::string&);
bool validateDoubleStr(const std::string&);
bool validateIntStr(const std::string&);
bool isIdDuplicate(const std::string& id, const std::vector<ProductInfo>& inventory);