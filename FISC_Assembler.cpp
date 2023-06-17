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
#define R0 "00"
#define R1 "01"
#define R2 "10"
#define R3 "11"

struct instruction {
    string OpCode;
    string hexVal;
};

struct FileHandler {
    vector<string> File;
    ifstream inFS;
    ofstream outFS;
};

class Assembler {
public:
    struct FileHandler *F;
    map<string, int> LabelContent;
    string line;
    map<string, string> OpcodeRules;
    int DisplayType;
    char* outputFile;
    Assembler(struct FileHandler *F, int DisplayType, char* outputFile) {
        this->F = F;
        this->DisplayType = DisplayType;
        this->outputFile = outputFile;
        OpcodeRules.emplace("add", "00");
        OpcodeRules.emplace("and", "01");
        OpcodeRules.emplace("not", "10");
        OpcodeRules.emplace("bnz", "11");
        OpcodeRules.emplace("r0", "00");
        OpcodeRules.emplace("r1", "01");
        OpcodeRules.emplace("r2", "10");
        OpcodeRules.emplace("r3", "11");
    }
    string BinToHex(string OpCode){
        std::bitset<8> bitset(OpCode);
        int hexValue = bitset.to_ulong();
        stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << hexValue;
        return ss.str();
    }
    string DecToBin(string LabelName){
        int Labelnum;
        string binNo;
        auto iter1 = LabelContent.find(LabelName); 
        if (iter1 != LabelContent.end()){
            Labelnum = iter1->second; 
            while(Labelnum != 0){
                int rem = Labelnum % 2; 
                binNo.insert(0, to_string(rem)); 
                Labelnum /= 2;
            }
            if (binNo.length() < 6){
                while(binNo.length() < 6){ 
                    binNo.insert(0, "0");
                } 
            }
        } 
        else {
            binNo = "000000"; 
        }
        return binNo;
    }
    void AccessAssemblyFile() {
        while (getline(this->F->inFS, line)) {
            int foundIndex = line.find(';');
            if (foundIndex != string::npos) {
                if (foundIndex == 0){
                    continue;
                }
                line = line.substr(0, foundIndex-1);
                int print = true;
                unsigned int i = 0;
                while(i < line.length()){
                    if(line[i] != ' '){
                        print = true;
                        break;
                    }
                    print = false;
                    ++i;
                }
                if (!print){
                    continue;
                }
            }

            // Remove the trailing spaces
            while (line.back() == ' ') {
                line.pop_back();
            }
            this->F->File.push_back(line);
        }
    }
    string add_(string Operand){
        
        int startIndex = 0;
        int findIndex = Operand.find(" ");
        string Destination = Operand.substr(startIndex, findIndex);
        auto DestinationOp = OpcodeRules.find(Destination);
        startIndex = findIndex+1;
        findIndex = Operand.find(" ", findIndex+1);
        string Source1 = Operand.substr(startIndex, findIndex-startIndex);
        auto SourceOp1 = OpcodeRules.find(Source1);
        startIndex = findIndex+1;
        findIndex = Operand.find(" ", findIndex+1);
        string Source2 = Operand.substr(startIndex, findIndex-startIndex);
        auto SourceOp2 = OpcodeRules.find(Source2);

        return ADD + SourceOp1->second + SourceOp2->second + DestinationOp->second;
    }
    string and_(string Operand){
        int startIndex = 0;
        int findIndex = Operand.find(" ");
        string Destination = Operand.substr(startIndex, findIndex);
        auto DestinationOp = OpcodeRules.find(Destination);
        startIndex = findIndex+1;
        findIndex = Operand.find(" ", findIndex+1);
        string Source1 = Operand.substr(startIndex, findIndex-startIndex);
        auto SourceOp1 = OpcodeRules.find(Source1);
        startIndex = findIndex+1;
        findIndex = Operand.find(" ", findIndex+1);
        string Source2 = Operand.substr(startIndex, findIndex-startIndex);
        auto SourceOp2 = OpcodeRules.find(Source2);

        return AND + SourceOp1->second + SourceOp2->second + DestinationOp->second;
    }
    string not_(string Operand){
        int startIndex = 0;
        int findIndex = Operand.find(" ");
        string Destination = Operand.substr(startIndex, findIndex);
        auto DestinationOp = OpcodeRules.find(Destination);
        startIndex = findIndex+1;
        findIndex = Operand.find(" ", findIndex+1);
        string Source1 = Operand.substr(startIndex, findIndex-startIndex);
        auto SourceOp1 = OpcodeRules.find(Source1);
        return NOT + SourceOp1->second + "00" + DestinationOp->second;
    }
    string bnz_(string Operand){
        string LoopCode = DecToBin(Operand);
        return BNZ + LoopCode;
    }
    // This function is to detect all the labels and the find the line numbers of each label
    void firstPass(){
        int FileLine = 0;
        for (unsigned i = 0; i < this->F->File.size(); ++i){
            int labelFound = this->F->File[i].find(":");
            string Labelvalue;
            if (labelFound != -1){

                Labelvalue = this->F->File[i].substr(0, labelFound);
                LabelContent.emplace(Labelvalue, FileLine);
            }
            unsigned int j = 0;
            string withoutLabel = this->F->File[i].substr(labelFound+1);
            while(j < withoutLabel.length()){
                if(withoutLabel[j] != ' '){
                    break;
                }
                ++j;
            }
            this->F->File[i] = withoutLabel.substr(j);
            ++FileLine;
        }
        if (this->DisplayType == 4){
            cout << "*** LABEL LIST ***\n";
            for (auto &pair : LabelContent) {
                string key = pair.first;
                int value = pair.second;
                cout << key << " : " << value << endl;
            }
        }
        this->secondPass();
    }
    // This function has two uses: convert the instruction to opcode, convert the opcode to hex
    void secondPass() {
        struct instruction I[this->F->File.size()];
        // for (auto data : this->F->File) {
        //     cout << "data: " << data << data.length() << endl;
        // }
        for (unsigned int i = 0; i < this->F->File.size(); ++i){
            int startIndex = 0;
            int findIndex = this->F->File[i].find(" ");
            string Operator = this->F->File[i].substr(startIndex, findIndex - startIndex);
            string Operand = this->F->File[i].substr(findIndex+1);
            // if it's an add instruction
            if (Operator == "add"){
                I[i].OpCode = this->add_(Operand);
                I[i].hexVal = this->BinToHex(I[i].OpCode);
            }
            // if it's an and instruction
            else if (Operator == "and"){
                I[i].OpCode = this->and_(Operand);
                I[i].hexVal = this->BinToHex(I[i].OpCode);
            }
            // if it's a not instruction
            else if (Operator == "not"){
                I[i].OpCode = this->not_(Operand);
                I[i].hexVal = this->BinToHex(I[i].OpCode);
            }
            // if it's a bnz instruction
            else if (Operator == "bnz"){
                I[i].OpCode = this->bnz_(Operand);
                I[i].hexVal = this->BinToHex(I[i].OpCode);
            }
        }
        if (this->DisplayType == 4){
            cout << "*** MACHINE LIST ***\n";
            for (int i = 0; i < this->F->File.size(); ++i){
                cout << i << ":" << I[i].hexVal << "    " << this->F->File[i] << endl;
            }
        }
        if (this->DisplayType == 3){
            ofstream outFS(this->outputFile);
            outFS << "v2.0 raw\n";
            for (unsigned int i = 0; i < this->F->File.size(); ++i){
                outFS << I[i].hexVal << endl;
            }
            outFS.close();
        }
    }

};

int main(int argc, char* argv[]) {
    if (argc == 1){
        cerr << "USAGE: fiscas <source file> <object file> [-l]\n";
        return -1;
    }
    if (argc == 2){
        cerr << "USAGE: fiscas <source file> <object file> [-l]\n";
        return -2;
    }
    if (argc == 4){
        if (strcmp(argv[3], "-l") != 0){
           cerr << "USAGE: fiscas <source file> <object file> [-l]\n";
            return -3; 
        }
    }
    struct FileHandler F;
    F.inFS.open(argv[1]);
    if (!F.inFS) {
        cout << "Error opening input file." << endl;
        return 1;
    }
    Assembler* A = new Assembler(&F, argc, argv[2]);
    A->AccessAssemblyFile();
    A->firstPass();
    return 0;
}
