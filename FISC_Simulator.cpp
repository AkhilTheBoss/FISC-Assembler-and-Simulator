#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <bitset>
#include <sstream>
using namespace std;

#define ADD "00"
#define AND "01"
#define NOT "10"
#define BNZ "11"

struct FileHandler {
    vector<string> HexFile;
    ifstream inFS;
    ofstream outFS;
};

struct Instruction {
    string source_reg1;
    string source_reg2;
    string destination_reg;
    string full_instruction;
    int ZeroFlag;
};

class Splitter{
    public:
        vector<Instruction> Data;
        struct FileHandler *F;
        struct Instruction I;
        int DisplayType;
        string line;
        unsigned int ProgramCount;
        map<string, string> OpcodeRules;
        map<string, string> RegVals;
        map<string, string> DataInReg;
        string R0;
        string R1;
        string R2;
        string R3;
        vector<vector<string>> tokens;
        int PrintNoCycles;
        Splitter(struct FileHandler *F, int DisplayType, int PrintNoCycles){
            this->F = F;
            this->DisplayType = DisplayType;
            this->ProgramCount = 1;
            this->PrintNoCycles = PrintNoCycles;
            DataInReg.emplace("r0", "00000000");
            DataInReg.emplace("r1", "00000000");
            DataInReg.emplace("r2", "00000000");
            DataInReg.emplace("r3", "00000000");
            OpcodeRules.emplace("00", "add");
            OpcodeRules.emplace("01", "add");
            OpcodeRules.emplace("10", "not");
            OpcodeRules.emplace("11", "bnz");
            RegVals.emplace("00", "r0");
            RegVals.emplace("01", "r1");
            RegVals.emplace("10", "r2");
            RegVals.emplace("11", "r3");


        };
        void readHexFile(){
            while (getline(this->F->inFS, line)) {
                if (line == "v2.0 raw"){
                    continue;
                }
                this->F->HexFile.push_back(line);
            }

            this->processfile();
        }
        string binaryToHex(const string& binary) {
            // Convert binary string to an integer
            bitset<32> bits(binary);
            unsigned int intValue = bits.to_ulong();

            // Convert integer to hexadecimal string
            stringstream ss;
            ss << hex << intValue;
            string hexValue = ss.str();

            return hexValue;
        }
        string HexToBin(string hexString){
            // Convert hexadecimal to binary
            unsigned long hexValue = stoul(hexString, nullptr, 16);
            bitset<8> bitset(hexValue);
            string binaryString = bitset.to_string();
            return binaryString;        
        }
        void not_(string Operand){
            vector<string> vec;
            vec.push_back("not");
            auto source1 = RegVals.find(Operand.substr(0, 2));
            auto destination = RegVals.find(Operand.substr(4));
            vec.push_back(destination->second);
            vec.push_back(source1->second);
            this->tokens.push_back(vec);
        }
        void add_(string Operand){
            vector<string> vec;
            vec.push_back("add");
            auto source1 = RegVals.find(Operand.substr(0, 2));
            auto source2 = RegVals.find(Operand.substr(2, 2));
            auto destination = RegVals.find(Operand.substr(4));
            vec.push_back(destination->second);
            vec.push_back(source1->second);
            vec.push_back(source2->second);
            this->tokens.push_back(vec);
        }
        void and_(string Operand){
            vector<string> vec;
            vec.push_back("and");
            auto source1 = RegVals.find(Operand.substr(0, 2));
            auto source2 = RegVals.find(Operand.substr(2, 2));
            auto destination = RegVals.find(Operand.substr(4));
            vec.push_back(destination->second);
            vec.push_back(source1->second);
            vec.push_back(source2->second);
            this->tokens.push_back(vec);
        }
        void bnz_(string Operand){
            vector<string> vec;
            string loop;
            vec.push_back("bnz");
            loop = Operand.substr(0);
            int LoopNo = stoi(loop, nullptr, 2);
            vec.push_back(to_string(LoopNo));
            this->tokens.push_back(vec);
        }
        void processfile(){
            for (unsigned int i = 0; i < this->F->HexFile.size(); ++i){
                string binaryString = HexToBin(this->F->HexFile[i]);
                string Operator = binaryString.substr(0, 2);
                string Operand = binaryString.substr(2);
                if (Operator == NOT){
                    not_(Operand);
                }
                else if (Operator == AND){
                    and_(Operand);
                }
                else if (Operator == ADD){
                    add_(Operand);
                }
                else if (Operator == BNZ){
                    bnz_(Operand);
                }
            }
        }
        string processNot(string dest, string source){
            auto it = DataInReg.find(source);
            string source_reg1 = it->second;
            it = DataInReg.find(dest);
            string destination_reg = it->second;
            // Convert binary string to integer
            unsigned int value = bitset<8>(destination_reg).to_ulong();
            // Apply ~ operator
            unsigned int result = ~value;
            // Convert result back to binary string
            destination_reg = bitset<8>(result).to_string();

            return destination_reg;
        }
        string processAND(string dest, string source1, string source2){
            auto it = DataInReg.find(source1);
            string source_reg1 = it->second;
            it = DataInReg.find(source2);
            string source_reg2 = it->second;
            it = DataInReg.find(dest);
            string destination_reg = it->second;
            bitset<8> bits1(source_reg1);
            bitset<8> bits2(source_reg2);
            // Perform the bitwise AND operation
            bitset<8> result = bits1 & bits2;
            // Convert the result back to a binary string
            string binaryResult = result.to_string();
            destination_reg = binaryResult;
            return destination_reg;
        }
        string processADD(string dest, string source1, string source2){
            auto it = DataInReg.find(source1);
            string source_reg1 = it->second;
            it = DataInReg.find(source2);
            string source_reg2 = it->second;
            it = DataInReg.find(dest);
            string destination_reg = it->second;
            // Convert binary strings to integer values
            int num1 = stoi(source_reg1, nullptr, 2);
            int num2 = stoi(source_reg2, nullptr, 2);

            // Add the integer values
            int sum = num1 + num2;

            // Convert the sum back to a binary string
            string binarySum = bitset<8>(sum).to_string();

            // Remove leading zeros
            size_t firstNonZero = binarySum.find_first_not_of('0');
            if (firstNonZero != std::string::npos) {
                binarySum = binarySum.substr(firstNonZero);
            } else {
                binarySum = "00000000";
            }
            destination_reg = binarySum;
            return destination_reg;
        }
        int processBNZ(int ZVal, int no){
            int Future_PC;
            if (ZVal == 1){
                return -1;
            }
            Future_PC = stoi(this->tokens[no][1]);
            return Future_PC;
        }
        void PrintCycles(){
            string dest;
            int ZVal = 0;
            int count = 0;
            int i = 0;
            int x = 0; 
            int *counter = 0;
            if (this->PrintNoCycles != 0){
                x = this->PrintNoCycles;
                counter = &i;
                
            }
            else{
                x = static_cast<int>(this->F->HexFile.size());
                counter = &count;
            }
            while (x != *counter){
                // cout << "COUNT: " << count << endl;
                int F_PC = -1;
                cout << "Cycle: " << i+1 << " State:PC:" << this->ProgramCount;
                if (this->tokens[count][0] == "not"){
                    dest = processNot(this->tokens[count][1], this->tokens[count][2]);
                    DataInReg[this->tokens[count][1]] = dest;
                }
                if (this->tokens[count][0] == "and"){
                    dest = processAND(this->tokens[count][1], this->tokens[count][2], this->tokens[count][3]);
                    DataInReg[this->tokens[count][1]] = dest;
                }
                if (this->tokens[count][0] == "add"){
                    dest = processADD(this->tokens[count][1], this->tokens[count][2], this->tokens[count][3]);
                    DataInReg[this->tokens[count][1]] = dest;
                }
                if (this->tokens[count][0] == "bnz"){
                    F_PC = processBNZ(ZVal, count);
                }
                if (dest != "00000000"){
                    ZVal = 0;
                }
                else{
                    ZVal = 1;
                }
                cout << " Z: " << ZVal << " R0: " << binaryToHex(DataInReg["r0"]) << " R1: " << binaryToHex(DataInReg["r1"]) << " R2: " << binaryToHex(DataInReg["r2"]) << " R3: " << binaryToHex(DataInReg["r3"]) << endl;
                cout << "Disassembly: "; 
                for (int j = 0; j < this->tokens[count].size(); ++j){
                    cout << this->tokens[count][j] << " ";
                }
                cout << endl;
                ++this->ProgramCount;
                ++count;
                if (F_PC != -1){
                    this->ProgramCount = F_PC+1;
                    count = F_PC;
                }
                ++i;
            }

        }
};


int main(int argc, char* argv[]){
    struct FileHandler F;
    F.inFS.open(argv[1]);
    int PrintNoCycles;
    if (!F.inFS) {
        cout << "Error opening input file." << endl;
        return 1;
    }

    if (argc == 3){
        PrintNoCycles = atoi(argv[2]);
    }
    else {
        PrintNoCycles = 0;
    }
    Splitter* S = new Splitter(&F, argc, PrintNoCycles);
    S->readHexFile();
    S->PrintCycles();
    return 0;
}
