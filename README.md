# File Encryptor

## Description
File Encryptor is a program that converts a file into another file that is unreadable and incomprensible by using symetric encryption, so that the secret in the file is undoubtedly safe, and only user with the key is able to decrypt and access the content of the file. The program uses CLI to receive input, and you can check the command list in 

## Installation
1. Fork the repository at https://github.com/somanithpreap/file-encryptor
2. Clone the repo
```
git clone <github_username>/file-encryptor
```
3. Make sure you have C/C++ compiler and a code editor of your preference installed on your local machine
Checkout https://www.mingw-w64.org/downloads/ for downloading compiler and header files.
4. Run the project using main
For linux:
```
path$ bin/main
```
For Windows:
```
PS path> ./main
```

## Project Structure
```
/
|
+--- bin/
|    [compiled binaries are stored here]
|
+--- src/
|    |
|    +--- aes.cpp
|    +--- aes.h
|    +--- file-handler.cpp
|    +--- file-handler.h
|    +--- main.cpp
|    +--- test.cpp
|    +--- utils.cpp
|    +--- utils.h
|
+--- Makefile
+--- README.md
```
<br>

## Make
```make``` - compiles both the main program and the test program <br>
```make build``` - compiles the main program <br>
```make test``` - compiles the test program <br>
```make clean``` - delete the object files and test program <br>
```make clean-all``` - delete all object and binary files <br>
<br>

## Program Usage
1. CLI argument
Usage: file-encryptor -k <key> -e/-d [-r] <file_1/folder_1> <file_2/folder_2> ... <file_n/folder_n>
Flags:
-k, --key      Specify the key for the encryption and decryption process
-e, --encrypt  Set the program to encrypt files
-d, --decrypt  Set the program to decrypt files
-r, --recurse  Process all file(s) and subfolder(s) in the specified folder(s)

-h, --help     Display this help menu

## Program Structure
The program consists of three main parts:
- CLI Handler
- File Handler API
- Encryption Algorithm API

### The State
```
class State {
private:
  uint8 state[4][4];

public:
  State();
  State(uint8 in[16]);
  void update(uint8 in[16]);
  uint8 get_byte(uint8 row_i, uint8 col_i);
  void get_word(uint8 index, uint8 holder[4]);
  void get_state(uint8 holder[4][4]);
  void set_byte(uint8 row_i, uint8 col_i, uint8 value);
  void serialize(uint8 holder[16]);

  void add_round_key(uint8 r_key[16]);
  void sub_bytes();
  void shift_rows();
  void mix_columns();
  ~State();
};
```
