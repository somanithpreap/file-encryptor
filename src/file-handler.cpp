#include <iostream>
#include <vector>
#include <fstream>
#include <cstring> // For memcpy
#include "file-handler.h"


// Function to convert a simple passphrase to a byte array
std::vector<uint8> passphrase_to_bytes(const std::string &passphrase) {
    std::vector<uint8> bytes(passphrase.begin(), passphrase.end());
    return bytes;
}

void process_file(const char *file_path, const uint8 *key, uint8 k_len, char op_type);

int main() {
    // Input the key (passphrase)
    std::cout << "Enter key (passphrase): ";
    std::string passphrase;
    std::cin.ignore(); // To ignore the newline character left in the buffer
    std::getline(std::cin, passphrase);

    // Convert passphrase to byte array
    std::vector<uint8> key = passphrase_to_bytes(passphrase);
    uint8 k_len = key.size();

    // Input the operation type
    char op_type;
    std::cout << "Enter operation type (e for encrypt, d for decrypt): ";
    std::cin >> op_type;

    // Input the file path
    std::string file_path;
    std::cout << "Enter file path: ";
    std::cin >> file_path;

    // Process the file
    process_file(file_path.c_str(), key.data(), k_len, op_type);

    return 0;
}

void process_file(const char *file_path, const uint8 *key, uint8 k_len, char op_type) {
    // Open the input file in binary mode
    std::ifstream infile(file_path, std::ios::binary);
    if (!infile) {
        std::cerr << "Unable to open file: " << file_path << std::endl;
        return;
    }

    // Determine the file size
    infile.seekg(0, std::ios::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // Create a buffer to hold the file data
    std::vector<uint8> buffer(file_size);
    infile.read(reinterpret_cast<char*>(buffer.data()), file_size);
    infile.close();

    /// Perform AES operation (Encrypt/Decrypt)
   AES<16> aes(buffer.data());
   std::vector<uint8> output_buffer(file_size);
   
   uint8 round_key[6 + k_len / 4][4][4];
   key_expansion(k_len,key, round_key);

    if (op_type == 'e') {
        aes.encrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key);
    } else if (op_type == 'd') {
        aes.decrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key);
    } else {
        std::cerr << "Invalid operation type." << std::endl;
        return;
    }

    // Write the output to a new file
    std::string output_file_path = std::string(file_path) + ((op_type == 'e') ? ".enc" : ".dec");
    std::ofstream outfile(output_file_path.c_str(), std::ios::binary);
    outfile.write(reinterpret_cast<char*>(output_buffer.data()), output_buffer.size());
    outfile.close();
}
