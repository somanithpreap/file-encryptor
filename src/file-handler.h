#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <fstream>  // For file input/output operations
#include <vector>   // For using std::vector to manage dynamic arrays
#include <string>   // For handling file paths and string manipulation
#include <thread>   // For multithreading support

#include "aes.h"    // Include the AES encryption/decryption functionality
#include "utils.h"  // Include utility functions, such as error handling or key expansion



// Function prototypes
void readFile(std::ifstream& infile, std::vector<char>& buffer);
void process_file(const std::string &file_path, const uint8 *key, uint8 key_len, char op_type);



#endif  // End of header guard