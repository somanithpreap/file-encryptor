#include "file-handler.h"  // Including the header file that contains declarations for file handling and AES operations
#include <vector>
#include <thread>
#include <fstream>

void process_file(const char *file_path, uint8 *key, uint8 k_len, char op_type) {
    // Open the input file in binary mode
    std::ifstream infile(file_path, std::ios::binary);
    if (!infile) {
        ERROR("Unable to open file: %s", file_path);  // If the file cannot be opened, display an error message
    }

    // Determine the file size by moving the read pointer to the end of the file
    infile.seekg(0, std::ios::end);
    size_t file_size = infile.tellg();  // Get the file size in bytes
    infile.seekg(0, std::ios::beg);  // Move the read pointer back to the beginning of the file

    // Create a buffer to hold the file data
    std::vector<uint8> buffer(file_size);
    infile.read(reinterpret_cast<char*>(buffer.data()), file_size);  // Read the file content into the buffer
    infile.close();  // Close the input file

    // Perform AES operation (Encrypt/Decrypt)
    AES<16> aes(buffer.data());  // Initialize the AES object with the buffer data
    std::vector<uint8> output_buffer(file_size);  // Create an output buffer of the same size as the input

    uint8 round_key[6 + k_len / 4][4][4];  // Allocate memory for the round keys (adjust the size as needed for AES)
    key_expansion(k_len, key, round_key);  // Perform key expansion to generate the round keys

    if (op_type == 'e') {
        aes.encrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key);  // Encrypt the file data
    } else if (op_type == 'd') {
        aes.decrypt(buffer.data(), buffer.data(), output_buffer.data(), round_key);  // Decrypt the file data
    } else {
        ERROR("Invalid operation type.");  // If the operation type is invalid, display an error message
    }

    // Write the output to a new file (either .enc for encryption or .dec for decryption)
    std::ofstream outfile((std::string(file_path) + ((op_type == 'e') ? ".enc" : ".dec")).c_str(), std::ios::binary);
    outfile.write(reinterpret_cast<char*>(output_buffer.data()), output_buffer.size());  // Write the output data to the file
    outfile.close();  // Close the output file
}

void process_files(std::vector<char *> &files, uint8 *key, uint8 k_len, char op_type) {
    size_t num_files = files.size();  // Get the number of files to process
    std::vector<std::thread> threads;  // Create a vector to hold the threads

    // Process files in batches of 4
    for (size_t i = 0; i < num_files; i += 4) {
        for (size_t j = 0; j < 4 && (i + j) < num_files; ++j) {
            // Create a thread for each file, up to 4 files at a time
            threads.push_back(std::thread(process_file, files[i + j], key, k_len, op_type));
        }

        // Wait for all threads in the current batch to finish
        for (std::thread &t : threads) {
            if (t.joinable()) {
                t.join();  // Join the thread (wait for it to finish)
            }
        }
        threads.clear();  // Clear the thread vector for the next batch of files
    }
}
