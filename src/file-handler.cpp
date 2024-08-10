#include <iostream>   // For standard input/output operations
#include <vector>     // For using std::vector to handle dynamic arrays
#include <fstream>    // For file input/output operations
#include <cstring>    // For memory operations (e.g., memcpy)
#include <cstdint>    // For fixed-width integer types (e.g., uint8_t)
#include <thread>     // For multithreading operations
#include <mutex>      // For synchronization (not used in the current code but included for potential future use)
#include <sstream>    // For string stream operations
#include <ctime>      // For time-related functions
#include <filesystem> // For filesystem operations (C++17)
#include "aes.h"      // Assuming you have an AES header file for AES encryption/decryption functionality

#ifndef AES_H
#define AES_H

typedef uint8_t uint8;  // Define uint8 as a type alias for uint8_t

// Function prototypes
void process_file(const std::string &file_path, const uint8 *key, uint8 key_len, char op_type);
void process_files(const std::vector<std::string> &files, const uint8 *key, uint8 key_len, char op_type);
void process_directory(const std::string &directory_path, const uint8 *key, uint8 key_len, char op_type);

// Convert a simple passphrase to a byte array
std::vector<uint8> passphrase_to_bytes(const std::string &passphrase) {
    // Convert the string passphrase into a vector of bytes
    return std::vector<uint8>(passphrase.begin(), passphrase.end());
}

// Generate a unique file name based on the base name and extension
std::string generate_unique_file_name(const std::string &base_name, const std::string &extension) {
    std::ostringstream oss;          // Create a string stream to build the file name
    oss << base_name << "_" << time(nullptr) << extension; // Append base name, current time, and extension
    return oss.str();                // Return the generated unique file name
}

int main() {
    // Input the key (passphrase) from the user
    std::cout << "Enter key (passphrase): ";
    std::string passphrase;
    std::getline(std::cin, passphrase); // Read the entire line of input for the passphrase

    // Convert passphrase to byte array
    std::vector<uint8> key = passphrase_to_bytes(passphrase); // Convert the passphrase to bytes
    uint8 key_len = key.size();  // Get the length of the key

    // Input the operation type from the user
    char op_type;
    std::cout << "Enter operation type (e for encrypt, d for decrypt): ";
    std::cin >> op_type; // Read the operation type (either 'e' or 'd')

    // Input the file or directory path from the user
    std::string path;
    std::cout << "Enter file or directory path: ";
    std::cin >> path; // Read the path

    // Check if the path is a file or directory
    if (std::filesystem::is_directory(path)) {
        // If it's a directory, process all files within the directory
        process_directory(path, key.data(), key_len, op_type);
    } else if (std::filesystem::is_regular_file(path)) {
        // If it's a file, process the single file
        process_file(path, key.data(), key_len, op_type);
    } else {
        std::cerr << "Invalid path provided." << std::endl; // Print error message if the path is neither file nor directory
    }

    return 0; // End of the program
}

// Process a single file: either encrypt or decrypt based on op_type
void process_file(const std::string &file_path, const uint8 *key, uint8 key_len, char op_type) {
    // Open the input file in binary mode
    std::ifstream infile(file_path, std::ios::binary);
    if (!infile) {
        std::cerr << "Unable to open file: " << file_path << std::endl; // Print error message if file cannot be opened
        return;
    }

    // Determine the file size and read its contents into a buffer
    infile.seekg(0, std::ios::end);  // Move to the end of the file
    size_t file_size = infile.tellg(); // Get the file size
    infile.seekg(0, std::ios::beg);   // Move back to the beginning of the file

    std::vector<uint8> buffer(file_size); // Create a buffer to hold the file data
    infile.read(reinterpret_cast<char*>(buffer.data()), file_size); // Read file data into the buffer
    infile.close(); // Close the input file

    // Initialize AES encryption/decryption
    AES<16> aes(buffer.data()); // Create an AES object with the buffer data
    std::vector<uint8> output_buffer(file_size); // Buffer for the output data
    uint8 round_key[6 + key_len / 4][4][4]; // Array to hold the expanded key

    void key_expansion(uint8 key_len, const uint8 *key, uint8 (*holder)[4][4]);
    key_expansion(key_len, key, round_key); // Expand the key for AES

    // Perform AES operation based on op_type
    if (op_type == 'e') {
        aes.encrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key); // Encrypt the data
    } else if (op_type == 'd') {
        aes.decrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key); // Decrypt the data
    } else {
        std::cerr << "Invalid operation type." << std::endl; // Print error message if op_type is invalid
        return;
    }

    // Generate a unique output file name and write the processed data to it
    std::string output_file_path = generate_unique_file_name(file_path, (op_type == 'e') ? ".enc" : ".dec");
    std::ofstream outfile(output_file_path, std::ios::binary);
    if (!outfile) {
        std::cerr << "Unable to create output file: " << output_file_path << std::endl; // Print error message if output file cannot be created
        return;
    }
    outfile.write(reinterpret_cast<char*>(output_buffer.data()), output_buffer.size()); // Write the processed data to the output file
    outfile.close(); // Close the output file
}

// Recursively process all files in a directory
void process_directory(const std::string &directory_path, const uint8 *key, uint8 key_len, char op_type) {
    std::vector<std::string> files; // Vector to hold paths of files to be processed

    // Iterate through all entries in the directory
    for (const auto &entry : std::filesystem::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string()); // Add file path to the vector if it's a regular file
        } else if (entry.is_directory()) {
            process_directory(entry.path().string(), key, key_len, op_type); // Recursively process subdirectories
        }
    }

    // Process all collected files
    process_files(files, key, key_len, op_type);
}

// Process multiple files in parallel using threads
void process_files(const std::vector<std::string> &files, const uint8 *key, uint8 key_len, char op_type) {
    std::vector<std::thread> threads; // Vector to hold the threads
    size_t num_files = files.size();  // Get the number of files to process

    // Process files in batches of 4
    for (size_t i = 0; i < num_files; i += 4) {
        // Launch up to 4 threads for processing files
        for (size_t j = 0; j < 4 && (i + j) < num_files; ++j) {
            threads.push_back(std::thread(process_file, files[i + j], key, key_len, op_type)); // Create and start a new thread
        }

        // Wait for all threads in the current batch to finish
        for (std::thread &t : threads) {
            if (t.joinable()) {
                t.join(); // Wait for the thread to finish
            }
        }
        threads.clear(); // Clear the thread vector for the next batch
    }
}

#endif // AES_H
