# File Encryptor

## Description
File Encryptor is a program that converts a file into another file that is unreadable and incomprensible by using symetric encryption, so that the secret in the file is undoubtedly safe, and only user with the key is able to decrypt and access the content of the file. The program uses CLI to receive input, and you can check the command list in ## Program Usage.

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
Alternative way

  Install dependencies
  ```
  npm install
  ```
  Start the project
  ```
  npm start
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
1. CLI Argument
  Usage: file-encryptor -k <key> -e/-d [-r] <file_1/folder_1> <file_2/folder_2> ... <file_n/folder_n>
  Flags:
  -k, --key      Specify the key for the encryption and decryption process
  -e, --encrypt  Set the program to encrypt files
  -d, --decrypt  Set the program to decrypt files
  -r, --recurse  Process all file(s) and subfolder(s) in the specified folder(s)

  -h, --help     Display this help menu

2. File-Handler
  (To Be Updated)
3. State
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
    void print();

    void add_round_key(uint8 r_key[4][4]);
    void sub_bytes(bool inverse);
    void shift_rows(bool inverse);
    void mix_columns(bool inverse);
    ~State(){};
  };
```
  This class is responsible for taking the bytes imported by file-handler and arrange them into 4 * 4 array of bytes called "State". Creating "State" is evolved by various methods which have shown in the code snippet. For more information, check **aes.cpp** to see the usage of each function.

5. Key Expansion Algorithm
```
  void key_expansion(uint8 k_len, uint8 *key, uint8 (*holder)[4][4]);
```
  This function takes the format length of key which could be either 128 bits (16 characters long), 192 bits (24 characters long), or 256 bits (32 characters long), the actual key for encryption/decription, and data from holder array which is taken from State class after applying methods. It's resulting in more unique keys (the amount of key is based on the length of the key respectively, could be either 11 round keys, 13 round keys, or 15 round keys), and those keys are stored in 3D array "round_key". For more information, check the comments in **aes.cpp**.

4. AES Algorithm
```
  template <uint8 k_len> class AES { 
  public:
    AES(){};
    AES(uint8 in[16]);
    State state;

    void update(uint8 in[16]);
    static void encrypt(State state, uint8 data[16], uint8 holder[16],
                        uint8 round_key[(6 + k_len / 4) + 1][4][4]);
    static void decrypt(State state, uint8 data[16], uint8 holder[16],
                        uint8 round_key[(6 + k_len / 4) + 1][4][4]);
    ~AES(){};
  };
```
  This class is responsible for the implementation of AES algorithm, both encryption and decryption. It uses the data from from "State" and round key from "round_key", and applies methods on the data and key. After all the method application, it is stored in "holder" which is a holder variable for the encrypted (when "encrypt" function is used) or the decrypted (when "decrypt" function is used) data to be used again. For more information, check **aes.cpp**.

5. Code in main.cpp
  This program uses CLI (Command Line Interface) for interaction between user and the machine. We admit that it is not user friendly, but *It Has To Be This Way* due to time contraint. For command line and usage, checkout **1. CLI Argument**

6. Constant Variables in **aes.h**
  They are constant values written to use in methods that they are belonged to. Listen, **THEY MUST NOT BE TEMPERED BY ANYONE!** in order to make the program running as it should.
  - SBOX and InvSBOX (Substitution Box and Inverse Substitution Box) are used in "sub_byte" and "sub_word" function.
  - MIXCOL_MATRIX and InvMIXCOL_MATRIX are used in "mix_columns" function.
  - Rcon (Round Constant) is used "key_expansion" function.

7. **utils.h** & **utils.cpp**
  The functions in there are utilities for developer to replicate the action of manipulating bytes and checking for error etc.
  - "ERROR" function is for displaying error message and end the program if the program is not running as expected.
  - "CHECK_NULL_PTR" function is for checking if a pointer contains null pointer. If it does, it displays error message.
  - "CHECK_NON_ZERO_BUFFER" function is for checking if the buffer has data or not. If it does not, it displays error message.
  - "display_buffer" function is for displaying data of the buffer.
  - "l_rotate_word" function is for shifting bytes to the left based on the input. It takes a 4 bytes array and number of how far you want to shift a byte.
  - "r_rotate_word" function is for shifting bytes to the right based on the input. It takes a 4 bytes array and number of how far you want to shift a byte.

## Program Structure
The program consists of three main parts:
```
+------------------------------------+
|       +------------------+         |
|       |   CLI Handler    |         |
|       +------------------+         |
|                                    |
|      +---------------------+       |
| +~~~>|  File Handler API   |~~~~~+ |
| |    +---------------------+     | |
| |                                | | 
| | +----------------------------+ | |
| +~|  Encryption Algorithm API  |<+ |
|   +----------------------------+   |
+------------------------------------+
```

## Contributing
  1. Fork the Project
  2. Create your Feature Branch
  ```
    git checkout -b feature/AmazingFeature
  ```
  3. Commit your Changes 
  ```
    git commit -m 'Add some AmazingFeature'
  ```
  4. Push to the Branch 
  ```
    git push origin feature/AmazingFeature
  ```
  5. Open a Pull Request

## License
  Distributed under the MIT License. See `LICENSE.txt` for more information.

## Contact
  somanithpreap - <your_email>
  SoksophalSORN - ss6024010027@camtech.edu.kh
  menghorngkao -  <your_email>

  Project Link: https://github.com/somanithpreap/file-encryptor

## Acknowledgements
  - Contributor
  -> Project Manager: https://github.com/somanithpreap
  -> Contributor : https://github.com/menghorngkao
  -> Contributor : https://github.com/SoksophalSORN

  -C/C++ Programming Language
    This project utilizes C/C++ for its core functionality. Special thanks to the C and C++ communities for their robust programming languages and support.