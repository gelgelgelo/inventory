#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
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

/*  error messages for error handling, mostly gonna be used for inputs tho.
	how to use:
	 user inputted a letter in an int input: std::cout << errmsg.invalidIntType;
	 user inputted an invalid value for int: std::cout << errmsg.invalidIintRange;
	 etc. */
struct ErrMsgs {
	// Argument Errors
	const std::string missingArguments = "Argument Error: Arguments is incomplete, the program may not run properly.\n"
										 "Use the run command of the Makefile or run the code along with the file path of the text file.\n";

	// File Errors
	const std::string fileCantBeRead = "File Error: File cannot be read. Is the text file missing or renamed?\n"
									   "Make sure Inventory/inventory.txt exists.\n";
	const std::string corruptLineSkipped = "File Error: Line not parsed properly, perhaps the line was corrupted.\n";
    
	// --- Input Errors ---
	// Integer Errors
    const std::string invalidIntType  = "Input Error: Please enter a valid whole number.\n";
    const std::string invalidIntRange = "Input Error: Number is out of the allowed range.\n";

    // Double Errors
    const std::string invalidDoubleType  = "Input Error: Please enter a valid decimal number.\n";
    const std::string invalidDoubleRange = "Input Error: Value is out of the allowed range.\n";

    // String Errors
	const std::string invalidStrLength = "Input Error: Text length does not meet the required limits.\n";
    const std::string invalidStrPipePresent = "Input Error: Pipe ('|') was detected. Please avoid this character\n";

	// Character Errors
    const std::string invalidCharType = "Input Error: Please enter only a single character.\n";
	const std::string invalidCharVal  = "Input Error: That choice is not recognized.\n";

	// --------------------
}const errmsg;


// PROTOTYPES

// file handling
bool loadInventory(std::vector<ProductInfo>&, const std::string&);
ProductInfo parseLine(const std::string&);

// inputs
std::string getString(const std::string&, int, int);
double getDouble(const std::string&, double, double);
int getInt(const std::string&, int, int);
char getChar(const std::string&, char, char);
bool validateString(const std::string&);
bool validateDoubleStr(const std::string&);
bool validateIntStr(const std::string&);



 // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< MAIN LOGIC FLOW

// this uses args so we can run 
int main(int argc, char* argv[])
{
	// check if the program is being called properly
	if(argc < 2){
		std::cout << errmsg.missingArguments;
		return 1;
	}
	

	// initialize inventory vector to prepare for loading
	std::vector<ProductInfo> productInventory;

	// check if the file was read properly
	std::string filePath = argv[1];
	if(!loadInventory(productInventory, filePath)){
		std::cout << errmsg.fileCantBeRead;
		return 1;
	}

	return 0;
}


bool loadInventory(std::vector<ProductInfo>& productInventory, const std::string& filePath){
	if(filePath.empty()) return false;

	std::string productRawData;
	ProductInfo processedData;

	std::ifstream readFile(filePath);
	if(!readFile.is_open()) return false;

	while (std::getline (readFile, productRawData)) {
		if(productRawData.empty()) continue;

		processedData = parseLine(productRawData);

		if(processedData.ID == "[ERROR]"){
			std::cout << errmsg.corruptLineSkipped;
			std::cout << "Raw Line : " << productRawData << "\n";
			continue;
		}

		productInventory.push_back(processedData);
	}
	readFile.close(); // close the file

	return true;
}

ProductInfo parseLine(const std::string& productRawData){
	std::string temp = "", ID, name;
	double price;
	int stockQnty, dataReading = 0;
	bool lineParsed = true;

	for(char c : productRawData){
		// append to temp any non pipe char
		if(c != '|'){
			temp += c;
			continue;
		}

		// process the data once it reaches a pipe
		if(temp.empty()){
			lineParsed = false;
			break;
		}
		
		if(dataReading == 0) ID = temp;
		else if(dataReading == 1) name = temp;
		
		else if(dataReading == 2){
			if(!validateDoubleStr(temp)){
				lineParsed = false;
				break;
			}
			price = std::stod(temp);
		}
		else if(dataReading == 3) {
			if(!validateIntStr(temp)) {
				lineParsed = false;
				break;
			}
			stockQnty = std::stoi(temp);
		}

		// if properly processed, clear the temp and increment the data to be read
		dataReading++;
		temp.clear();
	}

	// possible case that a pipe was removed or added, this makes it return an error
	if(!lineParsed || dataReading != 4)
		return {"[ERROR]", "", 0.0, 0};
	else
		return {ID, name, price, stockQnty};
}


/*  input for values, last 2 params for min and max length/value
	1st is for the prompt to ask the user. 
	default params if not needed.
	how to use:
	std::string name = getString("Please enter product name.", 5, 32);
	double price = getDouble("Please enter product price.", 0.01);// notice max isnt given
	int ID = getInt("Please enter product ID.", 0);// same here
	char choice = getChar("Would you like to continue? (y/n)");// in this case, range limit wont make sense
																so simply allow the function asking for it 
																to proccess the choice
	[REMEMBER] make sure to check the minimum and maximum, you dont want that to mess
	up the program */
std::string getString(const std::string& prompt = "", int min = 1, int max = 999999){
	std::string input;

	while(true){
		std::cout << prompt << "\n >> ";
		std::getline(std::cin, input);
		if(!validateString(input)){
			std::cout << errmsg.invalidStrPipePresent;\
			continue;
		}
		if((int)input.length() < min || (int)input.length() > max){
			std::cout << errmsg.invalidStrLength;
			continue;
		}
		
		return input;
	}

	
}

double getDouble(const std::string& prompt = "", double min = 0.0, double max = 999999.9){
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

int getInt(const std::string& prompt = "", int min = 0, int max = 999999){
	std::string strInput;

	while(true){
		std::cout << prompt << "\n >> ";
		std::getline(std::cin, strInput);
		if(!(int)validateDoubleStr(strInput)){
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

char getChar(const std::string& prompt = "", char min = 'a', char max = 'z'){
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

bool validateString(const std::string& str){
	//immediately return false if empty input
	if(str.empty()) return false;

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
