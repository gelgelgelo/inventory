#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

// STRUCTS AND GLOBALS

/*  error messages for error handling, mostly gonna be used for inputs tho.
	how to use:
	 user inputted a letter in an int input: std::cout << errmsg.invalidIntType;
	 user inputted an invalid value for int: std::cout << errmsg.invalidIintRange;
	 etc. */
struct ErrMsgs {
    // Integer Errors
    const std::string invalidIntType  = "Error: Please enter a valid whole number.\n";
    const std::string invalidIntRange = "Error: Number is out of the allowed range.\n";

    // Double Errors
    const std::string invalidDoubleType  = "Error: Please enter a valid decimal number.\n";
    const std::string invalidDoubleRange = "Error: Value is out of the allowed range.\n";

    // String Errors
	const std::string invalidStrLength = "Error: Text length does not meet the required limits.\n";
    
	// Character Errors
    const std::string invalidCharType = "Error: Please enter only a single character.\n";
	const std::string invalidCharVal  = "Error: That choice is not recognized.\n";

}const errmsg;


// PROTOTYPES
std::string getString(const std::string&, int, int);
double getDouble(const std::string&, double, double);
int getInt(const std::string&, int, int);
char getChar(const std::string&, char, char);
bool validateDoubleStr(const std::string&);



 // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< MAIN LOGIC FLOW
int main(void)
{
	std::cout << "Inventory System\n";
	std::string str = getString("Input string.", 5, 10);
	double db = getDouble("Input double.", 0.0, 100.0);
	int i = getInt("Input int.", 0, 5);
	char ch = getChar("Input char.", 'a', 'c');

	std::cout << str << " " << db << " " << i << " " << ch << "\n";
	return 0;
}



/*  input for values, last 2 params for min and max length/value
	1st is for the prompt to ask the user. 
	default params if not needed.
	how to use:
	std::string name = getString("Please enter product name.", 5, 32);
	double price = getDouble("Please enter product price.", 0.01);// notice max isnt given
	int ID = getInt("Please enter product ID.", 0);// same here
	char choice = getChar("Would you like to continue? (y/n)");

	[REMEMBER] make sure to check the minimum and maximum, you dont want that to mess
	up with the program */
std::string getString(const std::string& prompt = "", int min = 1, int max = 999999){
	std::string input;

	while(true){
		std::cout << prompt << "\n >> ";
		std::getline(std::cin, input);
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