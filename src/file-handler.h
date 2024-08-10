#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <fstream>  // For file input/output operations
#include <vector>   // For using std::vector to manage dynamic arrays
#include <string>   // For handling file paths and string manipulation
#include <thread>   // For multithreading support

#include "aes.h"    // Include the AES encryption/decryption functionality
#include "utils.h"  // Include utility functions, such as error handling or key expansion

// Function to process a single file (either encrypt or decrypt)
void process_file(const char *file_path, uint8 *key, uint8 k_len, char op_type);

// Function to process multiple files, leveraging multithreading
void process_files(std::vector<char *> &files, uint8 *key, uint8 k_len, char op_type);



#endif  // End of header guard
