#include "Machine.h"

using namespace std;

// Definitions for global variables
map<string, string> reg;
map<string, string> Mem;
vector<string> instructions;
vector<string> operations;
vector<string> PC;
vector<int> Memory_print_add;

// Implementation of Register class
Register::Register() {
    address = "";
    value = "";
}

Register::Register(string add, string val) {
    address = add;
    value = val;
    reg[address] = value;
}

void Register::set_reg(string add, string val) {
    address = add;
    value = val;
    reg[add] = val;
}

string Register::get_reg(string add) const {
    auto it = reg.find(add);
    if (it != reg.end()) {
        return it->second;
    } else {
        return "";
    }
}

// Implementation of Memory class
Memory::Memory() {
    address = "";
    value = "";
}

Memory::Memory(string add, string val) {
    address = add;
    value = val;
    Mem[address] = value;
}

void Memory::set_memory(string add, string val) {
    address = add;
    value = val;
    Mem[add] = val;
}

string Memory::get_memory(string add) const {
    auto it = Mem.find(add);
    if (it != Mem.end()) {
        return it->second;
    } else {
        return "";
    }
}

// Implementation of ALU class methods
int ALU::hex_to_dec(const std::string& hex) {
    int dec_value = 0;
    int base = 1; // Base is initially 1 (16^0)

    // Traverse the hexadecimal string from right to left
    for (int i = hex.size() - 1; i >= 0; --i) {
        char ch = hex[i];

        // Convert hexadecimal character to decimal value
        if (isdigit(ch)) {
            dec_value += (ch - '0') * base;
        }
        else if (ch >= 'A' && ch <= 'F') {
            dec_value += (ch - 'A' + 10) * base;
        }
        else if (ch >= 'a' && ch <= 'f') {
            dec_value += (ch - 'a' + 10) * base;
        }
        else {
            throw std::invalid_argument("Invalid hexadecimal character");
        }

        base *= 16; // Move to the next power of 16
    }

    return dec_value;
}

string ALU::dec_to_hex(int dec) {
    if (dec == 0) return "0";
    string hex_str;
    const char hex_chars[] = "0123456789ABCDEF";
    while (dec > 0) {
        hex_str = hex_chars[dec % 16] + hex_str;
        dec /= 16;
    }
    return hex_str;
}

double ALU::bin_to_dec(const std::string& binaryStr) {
    // Split the integer and fractional parts
    size_t point = binaryStr.find('.');
    string intPart = binaryStr.substr(0, point);
    string fracPart = (point != string::npos) ? binaryStr.substr(point + 1) : "";

    // Convert integer part
    double decimalValue = 0;
    int len = intPart.length();
    for (int i = 0; i < len; i++) {
        if (intPart[i] == '1') {
            decimalValue += pow(2, len - i - 1);
        }
    }

    // Convert fractional part
    len = fracPart.length();
    for (int i = 0; i < len; i++) {
        if (fracPart[i] == '1') {
            decimalValue += pow(2, -(i + 1));
        }
    }
    return decimalValue;
}

string ALU::hex_to_bin(const std::string &hex) {
    // Convert hex string (2 characters) to an integer
    int intValue = stoi(hex, nullptr, 16);

    // Extract sign, exp, and mantissa
    int sign = (intValue >> 7) & 0x1;         // 1 bit for sign
    int exp = (intValue >> 4) & 0x7;          // 3 bits for exp
    int mantissa = intValue & 0xF;            // 4 bits for mantissa

    // Convert parts to binary strings
    string signBit = bitset<1>(sign).to_string();
    string expBits = bitset<3>(exp).to_string();
    string MBits = bitset<4>(mantissa).to_string();

    // Combine to form the final binary representation
    return signBit + expBits + MBits;
}

string ALU::Int_to_bin(int intPart) {
    string intBinary = "";
    while (intPart > 0) {
        intBinary = to_string(intPart % 2) + intBinary;
        intPart /= 2;
    }
    return intBinary.empty() ? "0" : intBinary;
}

string ALU::Frac_to_bin(double fracPart, int precision) {
    string fracBinary = ".";
    int count = 0;
    while (fracPart > 0 && count < precision) {
        fracPart *= 2;
        int bit = static_cast<int>(fracPart);
        fracBinary += to_string(bit);
        fracPart -= bit;
        count++;
    }
    return fracBinary;
}

string ALU::Dec_to_bin(double num) {
    int intPart = static_cast<int>(num);
    double fracPart = num - intPart;

    string intBinary = Int_to_bin(intPart);
    string fracBinary = Frac_to_bin(fracPart); // Default precision used

    return intBinary + fracBinary;
}

string ALU::bin_to_hex(const string &binary) {
    string hex;

    // Make sure the binary string length is a multiple of 4
    int padding = 4 - (binary.size() % 4);
    string paddedBinary = (padding == 4) ? binary : string(padding, '0') + binary;

    // Process each 4-bit group
    for (int i = 0; i < paddedBinary.size(); i += 4) {
        string binaryChunk = paddedBinary.substr(i, 4);
        int intValue = bitset<4>(binaryChunk).to_ulong();
        hex += "0123456789ABCDEF"[intValue];
    }
    return hex;
}

void ALU::fill_4_zeros(string& M) {
    int len = M.size();
    for (int i = len; i < 4; i++) {
        M += "0";
    }
}

void ALU::fill_3_zeros(string& exp) {
    int len = exp.size();
    for (int i = len; i < 3; i++) {
        exp = "0" + exp;
    }
}

double ALU::HEX_to_float_dec(string hex_num) {
    string bin_num = hex_to_bin(hex_num);
    string new_bin;
    int sign = 1;
    if (bin_num[0] == '1') {
        sign = -1;
    }
    string exp = bin_num.substr(1, 3);
    string M = bin_num.substr(4, 7);
    int pow = bin_to_dec(exp) - 4;
    if (pow >= 0) {
        for (int i = 0; i < M.size(); i++) {
            new_bin += M[i];
            if (i == pow - 1) {
                new_bin += '.';
            }
        }
    }
    else {
        new_bin = "0.";
        for (int i = 0; i < abs(pow); i++) {
            new_bin += "0";
        }
        new_bin += M;
    }

    return bin_to_dec(new_bin) * sign;
}

string ALU::float_dec_to_Hex(double dec_num) {
    if (dec_num == 0) {
        return "00";
    }
   string sign = "0";
    if(dec_num < 0){
        sign = "1";
    }
    string bin_num = Int_to_bin(abs(dec_num));
    int pow = 4 , start = 0 ;
    string M;

    bool left_1 = false;
    for(int i = 0 ; i < bin_num.size() ; i++){
        if(bin_num[i] == '1'){
            left_1 = true;
        }
        if(bin_num[i] == '.'){
            start = i;
            break;
        }
    }

    if(left_1 == true){
        for(int i = 0 ; i < bin_num.size() ; i++){
            if(bin_num[i] != '.' ){
                M += bin_num[i];
            }
        }
        pow += start;
    }
    else{
        int end;
        for(int i = start +1 ; i < bin_num.size() ; i++ ){
            if(bin_num[i] == '1'){
                end = i;
                break;
            }
        }
        M =  bin_num.substr(start+1,end);
        pow -= (end - start - 1);

    }
    string exp = Int_to_bin(pow);
    string full_bin;
    fill_4_zeros(M);
    fill_3_zeros(exp);
    full_bin += sign + exp + M;
    return (bin_to_hex(full_bin));
}

// Implementation of Get class methods
void Get::get_file_instruction(string file_name) {
    string instruction;
    ifstream ins_file(file_name + ".txt");
    while (ins_file >> instruction) {
        instructions.push_back(instruction);
    }
    Put_COOO(instructions);
}

void Get::get_pc() {
    for (string s : instructions) {
        PC.push_back(s.substr(2, 3));
    }
}

void Get::get_operations() {
    for (string s : instructions) {
        operations.push_back(string(1, s[0]));
    }
}

void Get::Put_COOO(vector <string>& ins){
    if(ins[ins.size()-1] != "C000"){
       ins.push_back("C000");
    }
}

// Implementation of UI class methods
void UI::Get_file_name() {
    string file_name;
    cout << "Please Enter the file name: \n";
    cin >> file_name;
    cin.ignore();
    Get::get_file_instruction(file_name);
}

void UI::Get_instruction_terminal() {
    cout << "Please Enter the instructions\n";
    string instruction = "";
    while (instruction != "C000") {
        cin >> instruction;
        cin.ignore();
        instructions.push_back(instruction);
    }
}

void UI::Main_Menu() {
    while (true) {
        string choice;
        cout << "1- Load instruction file\n";
        cout << "2- Type instructions\n";
        cout << "3- Exit\n";
        cout << "--> ";
        getline(cin, choice);
        cout << endl;
        if (choice == "1") {
            Get_file_name();
            break;
        }
        else if (choice == "2") {
            Get_instruction_terminal();
            break;
        }
        else if (choice == "3") {
            cout << "Exiting.....";
            exit(0);
        }
        else {
            cout << "Invalid choice, " << "Please Enter 1 or 2 or 3\n" << endl ;
        }
    }
}

void UI::Display_registers() {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        string index = ALU::dec_to_hex(i);
        auto it = reg.find(index);
        if (it != reg.end()) {
            cout << YELLOW << "R" + index << RESET << " = " << reg[index] << "  ";
        }
        else {
            cout << YELLOW << "R" + index << RESET << " = 00  ";
        }
        count++;
        if (count % 4 == 0) {
            cout << endl;
        }
    }
}

void UI::Display_Memory() {
    int col = 1;
    int count = 0;
    cout << "   ";
    for(int i = 0 ; i < 16 ; i++){
        cout << ALU::dec_to_hex(i) << "   ";
    }
    cout << endl;
    cout << ALU::dec_to_hex(0) << "  ";
    for (int i = 0; i < 256 ; i++) {
        
        string mem_add = ALU::dec_to_hex(i);
        if(Mem[mem_add].empty()){
            Mem[mem_add] = "00";
        }
        cout << Mem[mem_add] << "  ";
        count++;
        if (count % 16 == 0) {
            cout << endl;
            if(col != 16){
                 cout << ALU::dec_to_hex(col) << "  ";
                 col++;
            }
         
        }
    }
}

// Implementation of CU class methods
void CU::load_reg_mem_1(string reg_address, string mem_address) {
    Memory M;
    Register R;
    string value = M.get_memory(mem_address);
    if (value.empty()) {
        value = "00";
    }
    R.set_reg(reg_address, value);
}

void CU::load_reg_2(string reg_address, string mem_address) {
    Register R;
    R.set_reg(reg_address, mem_address);
}

void CU::load_mem_reg_3(string reg_address, string mem_address) {
    Memory M;
    Register R;
    string value = R.get_reg(reg_address);
    if (value.empty()) {
        value = "00";
    }
    M.set_memory(mem_address, value);
    Memory_print_add.push_back(ALU::hex_to_dec(mem_address));
}

void CU::Move_reg_reg_4(string reg_address_1, string reg_address_2) {
    Register R;
    string value = R.get_reg(reg_address_1);
    if (value.empty()) {
        value = "00";
    }
    R.set_reg(reg_address_2, value);
}

void CU::Add_int_5(string reg_add_save, string reg_add1, string reg_add2) {
    Register R;
    string value1 = R.get_reg(reg_add1);
    if (value1.empty()) {
        value1 = "00";
    }
    int int_val1 = ALU::hex_to_dec(value1);

    string value2 = R.get_reg(reg_add2);
    if (value2.empty()) {
        value2 = "00";
    }
    int int_val2 = ALU::hex_to_dec(value2);

    // Perform addition
    int total = int_val1 + int_val2;

    // Handle 2's complement and limit to 2 hex digits (8 bits)
    if (total > 255) {  // More than two hex digits
        total -= 256;
    } else if (total < 0) {  // In case of negative results after wrap-around
        total += 256;
    }

    // Convert back to hex and ensure 2-digit formatting
    string tot = ALU::dec_to_hex(total);
    if (tot.size() < 2) {
        tot = "0" + tot;
    }
    R.set_reg(reg_add_save, tot);
}

void CU::Add_float_6(string reg_add_save, string reg_add1, string reg_add2) {
    Register R;
    string value1 = R.get_reg(reg_add1);
    if (value1.empty()) {
        value1 = "00";
    }
    double int_val1 = ALU::HEX_to_float_dec(value1);

    string value2 = R.get_reg(reg_add2);
    if (value2.empty()) {
        value2 = "00";
    }
    double int_val2 = ALU::HEX_to_float_dec(value2);

    double total = int_val1 + int_val2;
    string tot = ALU::float_dec_to_Hex(total);

    if (tot.size() < 2) {
        tot = "0" + tot;
    }
    R.set_reg(reg_add_save, tot);
}

void CU::Bitwise_or_7(string reg_result_add, string reg_1_add, string reg_2_add) {
    Register R;
    string value1 = R.get_reg(reg_1_add);
    if (value1.empty()) {
        value1 = "00";
    }
    int int_val1 = ALU::hex_to_dec(value1);
    string value2 = R.get_reg(reg_2_add);
    if (value2.empty()) {
        value2 = "00";
    }
    int int_val2 = ALU::hex_to_dec(value2);
    int total = int_val1 | int_val2;
    string tot;
    if (total < 16) {
        tot = "0" + ALU::dec_to_hex(total);
    }
    else {
        tot = ALU::dec_to_hex(total);
    }
    R.set_reg(reg_result_add, tot);
}

void CU::Bitwise_and_8(string reg_result_add, string reg_1_add, string reg_2_add) {
    Register R;
    string value1 = R.get_reg(reg_1_add);
    if (value1.empty()) {
        value1 = "00";
    }
    int int_val1 = ALU::hex_to_dec(value1);
    string value2 = R.get_reg(reg_2_add);
    if (value2.empty()) {
        value2 = "00";
    }
    int int_val2 = ALU::hex_to_dec(value2);
    int total = int_val1 & int_val2;
    string tot;
    if (total < 16) {
        tot = "0" + ALU::dec_to_hex(total);
    }
    else {
        tot = ALU::dec_to_hex(total);
    }
    R.set_reg(reg_result_add, tot);
}

void CU::Bitwise_Xor_9(string reg_result_add, string reg_1_add, string reg_2_add) {
    Register R;
    string value1 = R.get_reg(reg_1_add);
    if (value1.empty()) {
        value1 = "00";
    }
    int int_val1 = ALU::hex_to_dec(value1);
    string value2 = R.get_reg(reg_2_add);
    if (value2.empty()) {
        value2 = "00";
    }
    int int_val2 = ALU::hex_to_dec(value2);
    int total = int_val1 ^ int_val2;
    string tot;
    if (total < 16) {
        tot = "0" + ALU::dec_to_hex(total);
    }
    else {
        tot = ALU::dec_to_hex(total);
    }
    R.set_reg(reg_result_add, tot);
}

void CU::Rotate_A(string reg_address, int X) {
    Register R;
    string value = R.get_reg(reg_address);
    if (value.empty()) {
        value = "00";
    }
    int int_val = ALU::hex_to_dec(value);
    bitset<8> binary_val(int_val);
    bitset<8> rotated_val = (binary_val >> X) | (binary_val << (8 - X));
    int rotated_int_val = static_cast<int>(rotated_val.to_ulong());
    string rotated_hex = ALU::dec_to_hex(rotated_int_val);
    if (rotated_hex.size() < 2) {
        rotated_hex = "0" + rotated_hex;
    }
    R.set_reg(reg_address, rotated_hex);
}

bool CU::jump_D(string reg_address) {
    Register R;
    string value = R.get_reg(reg_address);
    if (value.empty()) {
        value = "00";
    }
    int int_val = ALU::hex_to_dec(value);
    int int_val_r0 = ALU::hex_to_dec(R.get_reg("0"));
    return int_val > int_val_r0;
}

// Implementation of Cpu class methods
void Cpu::Run() {
    Get::get_operations();
    Get::get_pc();
    CU c;
    Register R;
    cout << "-----------------Steps-------------------\n" << endl;
    for (int i = 0; i < instructions.size(); i++) {
        string current_inst = instructions[i];
        string operation = operations[i];
        string pc = PC[i];

        if (operation == "1" || operation == "2" || operation == "3") {
            string reg_add(1, current_inst[1]);
            string mem_add = pc;

            if (operation == "1") {
                c.load_reg_mem_1(reg_add, mem_add);
                cout << "Data in M" << mem_add << " Loaded in R" << reg_add << endl;
            }
            else if (operation == "2") {
                c.load_reg_2(reg_add, mem_add);
                cout << "Data " << mem_add << " Loaded in R" << reg_add << endl;
            }
            else if (operation == "3") {
                if (mem_add == "00") {
                    if (R.get_reg(reg_add).empty()) {
                        R.set_reg(reg_add, "00");
                    }
                    cout << "Data in R" << reg_add << " is " << R.get_reg(reg_add) << endl;
                }
                else {
                    c.load_mem_reg_3(reg_add, mem_add);
                    cout << "Data in R" << reg_add << " Loaded in M" << mem_add << endl;
                }
            }

        }
        else if (operation == "4") {
            string reg_add1(1, current_inst[2]);
            string reg_add2(1, current_inst[3]);
            c.Move_reg_reg_4(reg_add1, reg_add2);
            cout << "Data in R" << reg_add1 << " Loaded in R" << reg_add2 << endl;
        }

        else if (operation == "B") {
            string reg_add(1, current_inst[1]);
            bool found = false;
            int increment = 0;
            if (reg["0"] == reg[reg_add]) {
                for (int j = i; j < PC.size(); j++) {
                    if (pc == PC[j + 1]) {
                        found = true;
                        break;
                    }
                    increment++;
                }
                if (found) {
                    i += increment;
                }
            }
            cout << "Jump performed to instruction at PC " << PC[i] << endl;
        }

        else if (operation == "D") {
            string reg_add(1, current_inst[1]);
            bool found = false;
            int increment = 0;
            if (c.jump_D(reg_add) == true) {
                for (int j = i; j < PC.size(); j++) {
                    if (pc == PC[j + 1]) {
                        found = true;
                        break;
                    }
                    increment++;
                }
                if (found) {
                    i += increment;
                }
            }
            cout << "Jump performed to instruction at PC " << PC[i] << endl;
        }

        else if (operation == "C") {
            break;
        }
        else {
            string reg_result_add(1, current_inst[1]);
            string reg_add1(1, current_inst[2]);
            string reg_add2(1, current_inst[3]);
            if (operation == "7") {
                c.Bitwise_or_7(reg_result_add, reg_add1, reg_add2);
                cout << "Data in R" << reg_add1 << " ORing with R" << reg_add2 << " Saved in R" << reg_result_add << endl;
            }
            else if (operation == "8") {
                c.Bitwise_and_8(reg_result_add, reg_add1, reg_add2);
                cout << "Data in R" << reg_add1 << " Anding with R" << reg_add2 << " Saved in R" << reg_result_add << endl;
            }
            else if (operation == "9") {
                c.Bitwise_Xor_9(reg_result_add, reg_add1, reg_add2);
                cout << "Data in R" << reg_add1 << " Exclusive Oring with R" << reg_add2 << " Saved in R" << reg_result_add << endl;
            }
            else if (operation == "5") {
                c.Add_int_5(reg_result_add, reg_add1, reg_add2);
                cout << "Data in R" << reg_add1 << " added to R" << reg_add2 << " in R" << reg_result_add << endl;
            }
            else if (operation == "6") {
                c.Add_float_6(reg_result_add, reg_add1, reg_add2);
                cout << "Data in R" << reg_add1 << " added to R" << reg_add2 << " in R" << reg_result_add << endl;
            }
            else if (operation == "A") {
                c.Rotate_A(reg_result_add, ALU::hex_to_dec(reg_add2));
                cout << "Data in R" << reg_result_add << " Rotated " << endl;
            }
        }
    }
}

// Implementation of Machine class methods
void Machine::RunProgram() {
    while (true) {
        u.Main_Menu();
        C.Run();
        cout << "\n--------------Registers------------------\n\n";
        u.Display_registers();
        cout << "\n---------------------------Memory--------------------------------\n\n";
        u.Display_Memory();
        cout << "\n--------------------------------\n";
        cout << CYAN << "IR " << RESET << "= " << instructions[instructions.size() - 1] << endl;
        cout << CYAN << "PC " << RESET << "= " << instructions[instructions.size() - 1].substr(2, 3) << endl;
        cout << "--------------------------------\n";
        reg.clear();
        Mem.clear();
        instructions.clear();
        PC.clear();
        operations.clear();
        Memory_print_add.clear();
    }
}