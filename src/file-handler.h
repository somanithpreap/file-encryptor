#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <thread>
#include "aes.h"
using namespace std;
namespace fs = filesystem;

bool is_program_file(fs::path path);
void scan_directory(fs::path dir_path, vector<fs::path> &files, bool recurse);
void process_file(const fs::path &file_path, char op_type, uint8 k_len, uint8 *key);
