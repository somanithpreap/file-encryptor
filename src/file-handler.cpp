#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <thread>
#include <sstream>
#include <cstdio>
#include "aes.h"
#ifndef AES_H
#define AES_H


using namespace std;

// Function to read a file into a buffer
void readFile(ifstream& infile, vector<char>& buffer) {
    infile.seekg(0, ios::end);
    streamsize fileSize = infile.tellg();
    infile.seekg(0, ios::beg);

    while (fileSize > 0) {
        streamsize readSize = min(static_cast<streamsize>(1024), fileSize);
        buffer.resize(static_cast<size_t>(readSize));
        infile.read(buffer.data(), readSize);
        fileSize -= infile.gcount();
    }
    infile.close();
}

// Process a single file: either encrypt or decrypt based on op_type
void process_file(const string &file_path, char op_type, const uint8 *key, uint8 key_len) {
    // Open the input file in binary mode
    ifstream infile(file_path, ios::binary);
    if (!infile) {
        cerr << "Unable to open file: " << file_path << endl;
        return;
    }

    vector<uint8> buffer;
    readFile(infile, buffer);

    AES<16> aes(reinterpret_cast<uint8*>(buffer.data()));
    vector<uint8> output_buffer(buffer.size());
    uint8 round_key[6 + key_len / 4][4][4];
    void key_expansion(uint8 key_len, const uint8 *key, uint8 (*holder)[4][4]);
    key_expansion(key_len, key, round_key);


    if (op_type == 'e') {
        aes.encrypt(buffer.data(), output_buffer.data(), round_key);
        
        string output_file_path = file_path + ".enc";
        ofstream outfile(output_file_path, ios::binary);
        if (!outfile) {
            cerr << "Unable to create output file: " << output_file_path << endl;
            return;
        }
        outfile.write(reinterpret_cast<char*>(output_buffer.data()), output_buffer.size());
        outfile.close();
    } else if (op_type == 'd') {
        aes.decrypt(buffer.data(), output_buffer.data(), round_key);
        string original_file_path = file_path;
        ofstream outfile(original_file_path, ios::binary);
        if (!outfile) {
            cerr << "Unable to create output file: " << original_file_path << endl;
            return;
        }
        outfile.write(reinterpret_cast<char*>(output_buffer.data()), output_buffer.size());
        outfile.close();
        remove(file_path.c_str()); // Delete the .enc file after decryption
    } else {
        cerr << "Invalid operation type." << endl;
        return;
    }

}

// Process multiple files in parallel using threads
void process_files(const vector<string>& files, const uint8 *key, uint8 key_len, char op_type) {
    vector<thread> threads; // Vector to hold the threads
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
