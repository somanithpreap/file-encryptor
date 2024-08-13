#include "file-handler.h"
#include <cstdio>

bool is_program_file(fs::path path) {
  return (path.string() == "file-encryptor" || path.string() == "./file-encryptor" ||
      path.string() == "file-encryptor.exe" || path.string() == "./file-encryptor.exe");
}

void scan_directory(fs::path dir_path, vector<fs::path> &files, bool recurse) {
  if (recurse) {
    for (const auto &entry : fs::recursive_directory_iterator(dir_path))
      if (entry.is_regular_file() && !is_program_file(entry.path())) files.push_back(entry.path());
  } else {
    for (const auto &entry : fs::directory_iterator(dir_path))
      if (entry.is_regular_file() && !is_program_file(entry.path())) files.push_back(entry.path());
  }
}

// Process a single file: either encrypt or decrypt based on op_type
void process_file(const fs::path &file_path, char op_type, uint8 k_len, uint8 *key) {
  cout << "Processing: " << file_path.c_str();

  // Open the input file in binary mode
  ifstream in_file(file_path, ios::binary);
  if (!in_file)
    ERROR("Unable to open file: %s", file_path.c_str());

  // Determine file size
  in_file.seekg(0, ios::end);
  streamsize file_size = in_file.tellg();
  in_file.seekg(0, ios::beg);
  cout << '[' << file_size << "B] ..." << endl;

  AES aes_inst(k_len);
  uint8 round_key[7 + k_len / 4][4][4];
  key_expansion(k_len, key, round_key);

  fs::path tmp_file_path{file_path.string() + ".enc-tmp"};
  ofstream tmp_file(tmp_file_path, ios::binary);
  if (!tmp_file) ERROR("Unable to create output file: %s", tmp_file_path.c_str());

  while (file_size > 0) {
    uint8 used_blocks = 0;
    char* buffer[64];
    for (uint8 i = 0; i < 64; i++) {
      buffer[i] = (char*)malloc(16);
      for (uint8 j = 0; j < 16; j++) buffer[i][j] = 0;
    }

    // Read data from file and store in buffer variable
    for (; used_blocks < 64 && file_size > 0; used_blocks++) {
      streamsize read_size = min((streamsize)16, file_size);
      in_file.read(buffer[used_blocks], read_size);
      file_size -= in_file.gcount();
    }

    // Process data in buffer variable
    uint8 holder[16];
    for (uint8 i = 0; i < used_blocks; i++) {
      if (op_type == 'e') aes_inst.encrypt((uint8*)buffer[i], holder, round_key);
      else aes_inst.decrypt((uint8*)buffer[i], holder, round_key);
      display_buffer(holder);

      // Check for EOF
      uint8 w_len = 0;
      for (; w_len < 16; w_len++)
        if (file_size == 0 && !holder[w_len] && w_len > 0 && w_len < 15)
          if(holder[w_len - 1] == 0x0A && !holder[w_len + 1]) break;
      tmp_file.write((char*)holder, w_len);
    }
  }

  in_file.close();
  tmp_file.close();
  remove(file_path);  // Remove original file
  rename(tmp_file_path, file_path); // Rename the temp file to the original file
  cout << "DONE" << endl;
}
/*
// Process multiple files in parallel using threads
void process_files(const vector<string>& files, const uint8 *key, uint8 key_len, char op_type) {
  vector<thread> threads; // Vector to hold the threads
  size_t num_files = files.size();  // Get the number of files to process

  // Process files in batches of 4
  for (size_t i = 0; i < num_files; i += 4) {
    // Launch up to 4 threads for processing files
    for (size_t j = 0; j < 4 && (i + j) < num_files; ++j) {
      threads.push_back(thread(process_file, files[i + j], key, key_len, op_type)); // Create and start a new thread
    }
    // Wait for all threads in the current batch to finish
    for (thread &t : threads) {
      if (t.joinable()) {
        t.join(); // Wait for the thread to finish
      }
    }
    threads.clear(); // Clear the thread vector for the next batch
  }
}
*/
