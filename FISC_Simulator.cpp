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

class Splitter{
    public:
        struct FileHandler *F;
        int DisplayType;
        string line;
        unsigned int ProgramCount;
        map<string, string> OpcodeRules;
        string R0;
        string R1;
        string R2;
        string R3;
        vector<vector<string>> tokens;
        Splitter(struct FileHandler *F, int DisplayType){
            this->F = F;
            this->DisplayType = DisplayType;
            this->ProgramCount = 0;
            this->R0 = "00000000";
            this->R1 = "00000000";
            this->R2 = "00000000";
            this->R3 = "00000000";
            OpcodeRules.emplace("00", "add");
            OpcodeRules.emplace("01", "add");
            OpcodeRules.emplace("10", "not");
            OpcodeRules.emplace("11", "bnz");
            OpcodeRules.emplace("00", "r0");
            OpcodeRules.emplace("01", "r1");
            OpcodeRules.emplace("10", "r2");
            OpcodeRules.emplace("11", "r3");
        };
        void readHexFile(){
            while (getline(this->F->inFS, line)) {
                if (line == "v2.0 raw"){
                    continue;
                }
                this->F->HexFile.push_back(line);
            }

            for (unsigned int i = 0; i < this->F->HexFile.size(); ++i){
                cout << "File: " << this->F->HexFile[i] << endl;
            }
            this->processfile();
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
            string source1, destination;
            vec.push_back("not");
            source1 = Operand.substr(0, 2);
            vec.push_back(source1);
            destination = Operand.substr(4);
            vec.push_back(destination);

            this->tokens.push_back(vec);
        }
        void add_(string Operand){
            vector<string> vec;
            string source1, source2, destination;
            vec.push_back("add");
            source1 = Operand.substr(0, 2);
            vec.push_back(source1);
            source2 = Operand.substr(2, 4);
            vec.push_back(source2);
            destination = Operand.substr(4);
            vec.push_back(destination);

            this->tokens.push_back(vec);
        }
        void and_(string Operand){
            vector<string> vec;
            string source1, source2, destination;
            vec.push_back("and");
            source1 = Operand.substr(0, 2);
            vec.push_back(source1);
            source2 = Operand.substr(2, 4);
            vec.push_back(source2);
            destination = Operand.substr(4);
            vec.push_back(destination);

            this->tokens.push_back(vec);
        }
        void bnz_(string Operand){
            vector<string> vec;
            string loop;
            vec.push_back("bnz");
            loop = Operand.substr(0);
            vec.push_back(loop);
           
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
        void PrintCycles(vector<string> tokens){
            for (int i = 0; i < 10; ++i){
                cout << "Cycle: " << i << "State:PC:" << this->ProgramCount << "\n";
                ++this->ProgramCount;
            }
        }
};


int main(int argc, char* argv[]){
    struct FileHandler F;
    F.inFS.open(argv[1]);
    if (!F.inFS) {
        cout << "Error opening input file." << endl;
        return 1;
    }
    Splitter* S = new Splitter(&F, argc);
    S->readHexFile();
    return 0;
}
