# File Encryptor

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
*Program usage documentation...*

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
