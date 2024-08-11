# File Encryptor

## Description
File Encryptor is a program that converts a file into an unreadable and incomprensible file by using symmetric encryption, so that the secret in the file is undoubtedly cryptic, and only user with the key is able to decrypt and access the content of the file. This is a CLI program, and you can check the command listed in **Program Usage**.

##Table of Contents
  1. [Installation](#installation)
  2. [Usage](#usage)
  3. [Contributing](#contributing)
  4. [License](#license)
  5. [Acknowledgements](#acknowledgements)

## Installation
1. Fork the repository at https://github.com/somanithpreap/file-encryptor
2. Clone the repo
```
git clone https://github.com/<github_username>/file-encryptor
```
3. Make sure you have C/C++ compiler and a code editor of your preference installed on your local machine. Checkout https://www.mingw-w64.org/downloads/ for downloading compiler and header files. Compile the project using the following commands: <br>
  `make` - compiles both the main program and the test programs <br>
  `make build` - compiles the main program <br>
  `make test` - compiles the test programs <br>
  `make clean-t` - delete the test programs <br>
  `make clean` - delete all binary files

## Program Usage
```
bin/file-encryptor -k <key> -e/-d [-r] <file_1/folder_1> <file_2/folder_2> ... <file_n/folder_n>
```
-k, --key &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Specify the key for the encryption and decryption process <br>
-e, --encrypt &nbsp;Set the program to encrypt files <br>
-d, --decrypt &nbsp;Set the program to decrypt files <br>
-r, --recurse &nbsp;Process all file(s) and subfolder(s) in the specified folder(s) <br>
<br>
-h, --help &nbsp;&nbsp;&nbsp;&nbsp;Display this help menu <br>

## Project Structure
1. **File structure** <br>
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
|    +--- utils.cpp
|    +--- utils.h
|
+--- test/
|    |
|    +--- s-test.cpp
|    +--- m-test.cpp
|
+--- LICENSE
+--- Makefile
+--- README.md
```
2. **File Handler** <br>
  (To Be Updated)
3. **The State** <br>
```cpp
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

    void add_round_key(uint8 r_key[4][4]);
    void sub_bytes(bool inverse);
    void shift_rows(bool inverse);
    void mix_columns(bool inverse);
    ~State(){};
  };
```
  This class is responsible for taking the bytes imported by file-handler and arrange them into 4 * 4 array of bytes called "State". Creating "State" is evolved by various methods which have shown in the code snippet. For more information, check **aes.cpp** to see the usage of each function.

4. **Key Expansion Algorithm** <br>
```cpp
  void key_expansion(uint8 k_len, uint8 *key, uint8 (*holder)[4][4]);
```
  This function takes the format length of key which could be either 16, 24, or 32 bytes, the actual key for encryption/decription, and a holder array to store the round keys. It's resulting in more unique keys (the amount of key is based on the length of the key respectively, could be either 11 round keys, 13 round keys, or 15 round keys). The holder variable should have the size of **(6 + k_len / 4) + 1**. For more information, check the comments in **aes.cpp**.

5. **AES Algorithm** <br>
```cpp
  template <uint8 k_len> class AES {
  public:
    AES(){};
    State state;

    static void encrypt(State state, uint8 data[16], uint8 holder[16],
                        uint8 round_key[(6 + k_len / 4) + 1][4][4]);
    static void decrypt(State state, uint8 data[16], uint8 holder[16],
                        uint8 round_key[(6 + k_len / 4) + 1][4][4]);
    ~AES(){};
  };
```
  This class is responsible for the implementation of AES algorithm, both encryption and decryption. It uses the data from the State and round keys from **round_key**, and applies methods on the data and key. After all the processing, it is stored in "holder" which is a holder variable for the encrypted (when "encrypt" function is used) or the decrypted (when "decrypt" function is used) data. For more information, check **aes.cpp**.

6. **src/main.cpp** <br>
  This program uses CLI (Command Line Interface) for interaction between user and the machine. We admit that it is not user friendly, but *It Has To Be This Way* due to time contraint. For command line and usage, checkout **Program Usage**

7. Constant Variables in **src/aes.h** <br>
  They are constant values written to use in methods that they are belonged to. Listen, **THEY MUST NOT BE TAMPERED BY ANYONE!** in order to make the program running as it should.
  - **SBOX** and **InvSBOX** (Substitution Box and Inverse Substitution Box) are used in the **sub_byte()** function.
  - **MIXCOL_MATRIX** and **InvMIXCOL_MATRIX** are used in **mix_columns()** function.
  - **Rcon** (Round Constant) is used **key_expansion()** function.

8. **src/utils.cpp** <br>
  The functions in here are utilities for developer to manipulate bytes and checking for error, etc.
  - **ERROR()** function is for displaying error message and end the program if the program is not running as expected.
  - **CHECK_NULL_PTR()** function is for checking if a pointer given is a NULL pointer. If it does, it displays an error message.
  - **CHECK_NON_ZERO_BUFFER()** function is for checking if given buffer has data or not. If it does not, it displays an error message.
  - **display_buffer()** function is for displaying a 16 bytes long buffer.
  - **l_rotate_word()** function is for shifting bytes to the left based on the input. It takes a 4 bytes array and number of how far you want to shift a byte.
  - **r_rotate_word()** function is for shifting bytes to the right based on the input. It takes a 4 bytes array and number of how far you want to shift a byte.

## Program Structure
The program consists of three main parts:
```
+--------------------------------------+
|         +------------------+         |
|         |   CLI Handler    |         |
|         +------------------+         |
|                  |                   |
|                  V                   |
|        +--------------------+        |
| +~~~~~>|  File Handler API  |~~~~~~+ |
| |      +--------------------+      | |
| |                                  | |
| |  +----------------------------+  | |
| +~~|  Encryption Algorithm API  |<~+ |
|    +----------------------------+    |
+--------------------------------------+
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
  Distributed under the MIT License. See `LICENSE` for more information.

## Contact
  somanithpreap - somanithpreap@gmail.com <br>
  SoksophalSORN - ss6024010027@camtech.edu.kh <br>
  menghorngkao -  mk6024010034@camtech.edu.kh

  Project Link: https://github.com/somanithpreap/file-encryptor

## Acknowledgements
  - **Contributors** <br>
  => Project Manager: https://github.com/somanithpreap <br>
  => Contributor : https://github.com/menghorngkao <br>
  => Contributor : https://github.com/SoksophalSORN

  - **C/C++ Programming Language** <br>
    This project utilizes C/C++ for its core functionality. Special thanks to the C and C++ communities for their robust programming languages and support.

  - **National Institute of Standards and Technologies** <br>
    National Institute of Standards and Technology (2001) Advanced Encryption Standard (AES). (Department of Commerce, Washington, D.C.), Federal Information Processing Standards Publication (FIPS) NIST FIPS 197-upd1, updated May 9, 2023. https://doi.org/10.6028/NIST.FIPS.197-upd
