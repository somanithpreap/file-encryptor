#include "file-handler.h"
#include <cstring>

void print_help();

int main(int argc, char *argv[]) {
  // Command line arguments handling
  if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    print_help();
    return 0;
  }
  // Check if there are at least the minimum number of required arguments
  if (argc < 5) {
    print_help();
    ERROR("Invalid number of arguments.");
  }

  // Check for recursive processing flag
  bool recurse = false;
  char op_type = ' ';
  uint8 k_len = 0;
  uint8 *key;
  vector<string> files;

  for (uint8 i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--encrypt") || !strcmp(argv[i], "-d") || !strcmp(argv[i], "--decrypt")) {
      if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--encrypt")) op_type = 'e';
      else op_type = 'd';
      for (uint8 j = i + 1; j < argc; j++) {
        if (!strcmp(argv[j], "-e") || !strcmp(argv[j], "--encrypt") ||
            !strcmp(argv[j], "-d") || !strcmp(argv[j], "--decrypt"))
          ERROR("Invalid token: %s", argv[j]);
        if (!strcmp(argv[j], "-k") || !strcmp(argv[j], "--key"))
          break;

        if (!(!strcmp(argv[j], "-r") || !strcmp(argv[j], "--recurse"))) {
          files.push_back(argv[j]);
          i++;
        } else {
          recurse = true;
          i++;
          // End the files/folders name scanning if already read before -r
          if (files.size())
            break;
        }
      }
    } else if (!strcmp(argv[i], "-k") ||
               !strcmp(argv[i],
                       "--key")) { // Check validity of key and parse key
      if (k_len) {
        print_help();
        ERROR("Key already specified.");
      }
      char *tmp_key = argv[i + 1];
      while (*(tmp_key++) != 0)
        k_len++;
      if (k_len == 16 || k_len == 24 || k_len == 32)
        key = (uint8 *)argv[++i];
      else
        ERROR("Key must be 16, 24, or 32 bytes long.");
    } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      print_help();
      return 0;
    } else
      ERROR("Unexpected token: %s", argv[i]);
  }

  // Check for argument satisfaction
  if (!k_len) {
    print_help();
    ERROR("No key provided.");
  }
  if (op_type == ' ') {
    print_help();
    ERROR("No operation specified.");
  }
  if (!files.size()) {
    print_help();
    ERROR("No files specified.");
  }

  printf("Key[%hhu]: %s\n", k_len, key);
  printf("Operation: %s\n", (op_type == 'e') ? "Encrypt" : "Decrypt");
  printf("Recurse: %s\n", (recurse) ? "true" : "false");


  // File handling and processing
  vector<fs::path> all_files; // Vector to store all files to be processed
  for (string file : files) {
    fs::path path{file};
    if (fs::is_regular_file(path) && !is_program_file(path)) all_files.push_back(path);
    else scan_directory(path, all_files, recurse);
  }

  // Print the input file
  printf("Files:   ");
  for (uint8 i = 0; i < all_files.size(); i++)
    cout << all_files.at(i).c_str() << ' ';
  cout << endl;

  for (fs::path &file : all_files) process_file(file, op_type, k_len, key);

  return 0;
}

void print_help() {
  printf("AES File Encryptor\n");
  printf("Usage: file-encryptor -k <key> -e/-d [-r] "
         "<file_1/folder_1> <file_2/folder_2> ... <file_n/folder_n>\n");
  printf("Flags:\n");
  printf("-k, --key      Specify the key for the encryption and decryption "
         "process\n");
  printf("-e, --encrypt  Set the program to encrypt files\n");
  printf("-d, --decrypt  Set the program to decrypt files\n");
  printf("-r, --recurse  Process all file(s) and subfolder(s) in the specified "
         "folder(s)\n\n");
  printf("-h, --help     Display this help menu\n\n");
}
