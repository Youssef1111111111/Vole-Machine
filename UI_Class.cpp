#include <iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


map<string, string> reg;
map<string, string> Mem;
vector <string> instructions;
vector <string> operations;
vector <string> PC;
vector <int> Memory_print_add;

class Get{
    public:
        static void get_file_instruction(string file_name ){
            string instruction;
            ifstream ins_file (file_name+".txt");
            while(ins_file >> instruction){
                instructions.push_back(instruction);
            }
        }
        static void get_pc(){
            for(string s : instructions){
                PC.push_back(s.substr(2,3));
            }
        }
        static void get_operations(){
            for(string s : instructions){
                operations.push_back(string(1,s[0]));
            }
        }

};



class UI{
    public:
        void Get_file_name(){
            string file_name;
            cout << "Please Enter the file name: \n";
            cin >> file_name;
            cin.ignore();
            Get::get_file_instruction(file_name);
            
        }

        void Get_instruction_terminal(){
            cout << "Please Enter the instructions\n";
            string instruction ="";
            while(instruction != "C000"){
                cin >> instruction;
                cin.ignore();
                instructions.push_back(instruction);
            }
        }


        void Main_Menu(){
            while(true){
            string choice;
            cout << "1- Load instruction file\n";
            cout << "2- Type instructions\n";
            cout << "3- Exit\n";
            cout << "--> ";
            getline(cin , choice);
            if(choice == "1"){
                Get_file_name();
                break;
            }
            else if(choice == "2"){
                Get_instruction_terminal();
                break;
            }   
            else if(choice == "3"){
                cout << RED << "Exiting....." << RESET;
                exit(0);
            }
            else{
                cout << RED <<"Invalid choice, " << RESET << "Please Ennter 1 or 2 or 3\n";
            }       
            }
      
        }

        void Display_registers(){
            int count = 0;
            for(int i = 0 ; i < 16 ; i++){
                string index = ALU::dec_to_hex(i);
                auto it = reg.find(index);
                if (it != reg.end()) {
                 cout << YELLOW << "R"+index << RESET << " = " << reg[index] << "  ";
                }
                else{
                 cout << YELLOW << "R"+index << RESET << " = 00  "; 
                }
                count++;
                if(count % 4 == 0){
                    cout << endl;
                }
            }
        }

        void Display_Memory(){
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

   
     



};