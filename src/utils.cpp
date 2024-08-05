#include "utils.h"

void ERROR(const char *message, ...) {
  printf("[ERR] ");
  va_list arg;
  va_start(arg, message);
  vprintf(message, arg);
  va_end(arg);
  printf("\n");
  exit(-1);
}

void CHECK_NULL_PTR(void *ptr) {
  if (ptr)
    return;
  ERROR("CHECK_NULL_PTR(): NULL pointer given.");
}

bool CHECK_NON_ZERO_BUFFER(size_t length, uint8 *buf) {
  if (length == 0)
    ERROR("CHECK_NON_ZERO_BUFFER(): Buffer length cannot be zero.");
  CHECK_NULL_PTR(buf);

  bool non_zero = true;
  for (size_t i = 0; i < length; i++) {
    if (buf++) {
      non_zero = false;
      break;
    }
  }
  return non_zero;
}

void l_rotate_word(uint8 word[4], uint8 positions) {
  if (positions == 0)
    ERROR("l_rotate_word(): Positions to be shifted needs to be non-zero.");
  CHECK_NON_ZERO_BUFFER(4, word);
  positions %= 4;

  for (uint8 p = 0; p < positions; p++) {
    uint8 tmp = word[0];
    for (uint8 i = 0; i < 3; i++)
      word[i] = word[i + 1];
    word[3] = tmp;
  }
}

void r_rotate_word(uint8 word[4], uint8 positions) {
  if (positions == 0)
    ERROR("l_rotate_word(): Positions to be shifted needs to be non-zero.");
  CHECK_NON_ZERO_BUFFER(4, word);
  positions %= 4;

  for (uint8 p = 0; p < positions; p++) {
    uint8 tmp = word[3];
    for (uint8 i = 3; i > 0; i--)
      word[i] = word[i - 1];
    word[0] = tmp;
  }
}

//These are utilities for File Handling and AES
uint8 encrypt = 0; //When signal to encrypt, the value is 1
uint8 decrypt = 0; //When signal to decrypt, the value is 1
uint8 recursive = 0; //When signal to recurse, the value is 1
char *key = NULL; //This takes the key for encryption and decryption.
char *file_name = NULL; //It is receiving input
const char *delimiter = " "; //This signals there are different elements in file_name. For example, "file1 file2 file3" = 3 elements.
uint8 max_length = 0; //Maximum size of columns of files
uint8 max_row = 0; //Maximum size of rows of files
char **files = nullptr; //2D dynamic array that contains all the inputted name of files.
//Please free memory using the below codes after using it, File Handler
  /*
  for (unsigned int i = 0; i < max_row; i++) {
        delete[] files[i];
    }
  delete[] files;
  */

//Command Line Interface function taking argc and argv from main()
void CLI(int argc, char *argv[]) {
  short int opt = 0;
  //For getting the option
  while ((opt = getopt(argc, argv, "e:d:k:")) != -1) {
    switch (opt) {
      case 'e':
        encrypt = 1;
        //Providing suboption for e
        if (strcmp(optarg, "-r") == 0) {
          recursive = 1;
          file_name = argv[optind];
        } else if (optarg[0] == '-') {
          fprintf(stderr, "Invalid Syntax");
          exit(1);
        } else {
          file_name = optarg;
        }
        break;

      case 'd':
        decrypt = 1;
        file_name = optarg;
        break;

      case 'k':
        key = optarg;
        break;

      case '?':
        fprintf(stderr, "Unknown Command: -%c\n", optopt);
        exit(1);

      default :
        fprintf(stderr, "Usage: %s [-flag] [argument]\n\t-e\ttakes file name as argument for encryption. For multiple file name, they must be written in double quote\n\t\tEx: \"file1 file2 file3\"\n\t\t\t-r is a suboption for -e to recursively encrypt files in a folder (a folder name must be specified)\n\t-d\ttakes file name as argument for dencryption. For multiple file name, they must be written in double quote\n\t\tEx: \"file1 file2 file3\"\n\t-k\ttakes key for encryption/decryption as argument.\n", argv[0]);
        exit(1);
    }
  }

  //For Debugging purpose
  /*
  printf("\nKey: %s\n", key);
  printf("Encrypt: %d\n", encrypt);
  printf("Recurse: %d\n", recursive);
  printf("Decrypt: %d\n", decrypt);
  printf("File name: %s\n", file_name);
  */
  if (file_name) { //If file name is not null
    max_length = strlen(file_name); //Getting the maximum of character of file name, abit overkill
    uint8 r = 0;
    for (uint8 i = 0; i < max_length; i++) {
      if (file_name[i] == ' ') {
        r++;
      }
    }
    max_row = r + 1; //Getting the maximum row of files
  } else {
    fprintf(stderr, "No file name provided\n");
    exit(1);
  }

  files = new char*[max_row]; //Initializing files

  //Setting the size of each row of files array
  for (unsigned int i = 0; i < max_row; i++) {
    files[i] = new char[max_length + 1];
  }

  //Getting each file name of file_name and add it to files array
  char *token = strtok(file_name, delimiter);
  int counter = 0;
  while (token != NULL && counter < max_row) {
    strcpy(files[counter], token);
    token = strtok(NULL, delimiter);
    counter++;
  }

  //For debugging purpose
  /*
  for (int i = 0; i < max_row; i++) {
    printf("Filename %d: %s\n", i + 1, files[i]);
  }
  */
  /*
  for (unsigned int i = 0; i < max_row; i++) {
        delete[] files[i];
    }
  delete[] files;
  */

}