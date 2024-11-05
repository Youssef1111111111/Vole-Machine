#ifndef MACHINE_H
#define MACHINE_H

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <cmath>
using namespace std;

// Color codes (define macros)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Extern declarations for global variables
extern map<string, string> reg;
extern map<string, string> Mem;
extern vector<string> instructions;
extern vector<string> operations;
extern vector<string> PC;
extern vector<int> Memory_print_add;

class Register {
private:
    string address;
    string value;

public:
    Register();
    Register(string add, string val);
    void set_reg(string add, string val);
    string get_reg(string add) const;
};

class Memory {
private:
    string address;
    string value;

public:
    Memory();
    Memory(string add, string val);
    void set_memory(string add, string val);
    string get_memory(string add) const;
};

class ALU {
public:
    static int hex_to_dec(const string& hex);
    static string dec_to_hex(int dec);
    static double bin_to_dec(const string& binaryStr);
    static string hex_to_bin(const string &hex);
    static string Int_to_bin(int intPart);
    static string Frac_to_bin(double fracPart, int precision = 10);
    static string Dec_to_bin(double num);
    static string bin_to_hex(const string &binary);
    static void fill_4_zeros(string& M);
    static void fill_3_zeros(string& exp);
    static double HEX_to_float_dec(string hex_num);
    static string float_dec_to_Hex(double dec_num);
};

class Get {
public:
    static void get_file_instruction(string file_name);
    static void get_pc();
    static void get_operations();
    static void Put_COOO(vector <string>& ins);
};

class UI {
public:
    void Get_file_name();
    void Get_instruction_terminal();
    void Main_Menu();
    void Display_registers();
    void Display_Memory();
};

class CU {
public:
    void load_reg_mem_1(string reg_address, string mem_address);
    void load_reg_2(string reg_address, string mem_address);
    void load_mem_reg_3(string reg_address, string mem_address);
    void Move_reg_reg_4(string reg_address_1, string reg_address_2);
    void Add_int_5(string reg_add_save, string reg_add1, string reg_add2);
    void Add_float_6(string reg_add_save, string reg_add1, string reg_add2);
    void Bitwise_or_7(string reg_result_add, string reg_1_add, string reg_2_add);
    void Bitwise_and_8(string reg_result_add, string reg_1_add, string reg_2_add);
    void Bitwise_Xor_9(string reg_result_add, string reg_1_add, string reg_2_add);
    void Rotate_A(string reg_address, int X);
    bool jump_D(string reg_address);
};

class Cpu {
public:
    void Run();
};

class Machine {
private:
    Cpu C;
    UI u;
public:
    void RunProgram();
};

#endif // MACHINE_H