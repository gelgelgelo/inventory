#pragma once
#include <string>

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
    const std::string searchUnavailable = "[Notice]: Search unavailable. Database contains no item data tracks.\n";
    const std::string nameNotFound = "\n[Search Error]: No records matched string entry arrays.\n";
}const errmsg;