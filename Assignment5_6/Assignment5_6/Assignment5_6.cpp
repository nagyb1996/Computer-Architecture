// Assignment5_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

// create register file
int reg[32];
//create data memory
int dataMem[8];
// create PC, cycles, and branch
int PC = 0;
int branch = 0;
int cycles = 3; //  first instruction always takes 3 cycles to fetch, decode and execute
// create instruction memory, decoded instruction and current instruction
vector<string> instructionMem;
vector<string> decodedInstruction;
vector<string> decodedNextInstruction;
vector<string> decodedPreviousInstruction;
string currentInstruction;
string previousInstruction;
string nextInstruction;
bool previousBubble = false; // track if there was a bubble in previous execution


//vector of possible instruction types
vector<string> instructionType = { "ADDI","ADD", "SUBI", "SUB", "DMEM", "LDUR", "STUR", "CBZ", "B" };

// Given a string content, and a string delimiter, split the string into a vector, and remove delimters
vector<string> split(string content, string delim) {
	vector<string> inits;

	int position = 0;
	string token;

	while ((position = content.find(delim)) != string::npos) { //split strings based on supplied delim token
		token = content.substr(0, position);
		inits.push_back(token);
		content.erase(0, position + delim.length());
	}
	inits.push_back(content); //Add whatever is left, with no delimeters, fixes issue of dropping final state

	return inits;
}

// returns value of register specified in passed arguement
string readReg(string regToRead) {
	int regInt = stoi(regToRead); // int value of reg to read
	int value = reg[regInt]; // value in reg
	string regString = to_string(value); // string of value in reg
	return regString;
}

// returns value of data memory specified in passed arguement
string readDataMem(string memToRead) {
	int memInt = stoi(memToRead);  // int value of dataMem to read
	int value = dataMem[memInt]; // value in dataMem
	string dataString = to_string(value); // string of value in dataMem
	return dataString;
}


// writes value to registers
void writeReg(int regID, int value) {
	reg[regID] = value; // update value in reg with provided value
}

// writes values to data memory
void writeDataMem(int memID, int value) {
	dataMem[memID] = value; // update value in dataMem with provided value
}


//Decode instruction
vector<string> decodeInstruction (string instruction) {
	// Find position of ' ' using find() 
	int space = instruction.find(" ");
	// Copy the substring after instruction
	string type = instruction.substr(0, space);
	vector<string> decoded; // decoded instruction to return
	string rawString; // variable to manipulate string
	string getValue; // variable to manipulate string
	vector<string> seperated; // variable to manipulate string
	
	// Add instruction type as first entry in decoded instruciton vector
	for (int i = 0; i < instructionType.size(); i++) {
		if (type.compare(instructionType[i]) == 0) {
			// put in the instruction and registers
			decoded.push_back(instructionType[i]);
			rawString = instruction.substr(space + 1);
			seperated = split(rawString, ",");
			for (int j = 0; j < seperated.size(); j++) {
				decoded.push_back(seperated[j]);
			}

			if (i == 0 || i == 2) { // if it is an I type
				decoded.push_back(readReg(decoded[2])); // source value
				decoded.push_back(readReg(decoded[3])); // destination value
				decoded.insert(decoded.begin(), "I");
			}
			else if (i == 1 || i == 3) { // if it is an R type
				decoded.push_back(readReg(decoded[1])); // 2nd value
				decoded.push_back(readReg(decoded[2])); // 1st value
				decoded.push_back(readReg(decoded[3])); // dest value
				decoded.insert(decoded.begin(), "R");
			}
			else if (i == 4) { // If it is DMEM
				decoded.push_back(decoded[1]); // location in data memory
				decoded.push_back(decoded[2]); // value to put in data memory
				decoded.insert(decoded.begin(), "P"); // P for psuedo
			}
			else if (i == 5 || i == 6) { // if it is an D type
				decoded.push_back(decoded[1]); // address
				decoded.push_back(readReg(decoded[2])); // base
				decoded.push_back(readReg(decoded[3])); // value in dest / source
				decoded.push_back(readDataMem(decoded[1])); // value in address
				decoded.insert(decoded.begin(), "D");
			}
			else if (i == 7) { // if it is an C type
				decoded.push_back(readReg(decoded[1])); // value to compare
				decoded.insert(decoded.begin(), "C");
			}
			else if (i == 8) { // if it is an B type
				decoded.insert(decoded.begin(), "B");
			}

		}
	}
	return decoded; // return the decoded instruction
}

// Method simulates ALU
int ALU(vector<string> instructionToExecute) {
	string typeOfInstruction = instructionToExecute[0]; // type of instuction
	string specificInstruction = instructionToExecute[1]; // specific instruction
	int result = 0; // hold result to write
	int destination = 0; // desination to write result
	int pcAddress = 0; // pcAddress to branch to if required
	bool writeToReg = true; // if we need to write to registers

	// R type instructions
	if (typeOfInstruction.compare("R") == 0) {
		int secondValue = stoi(instructionToExecute[5]); //get int value in second register
		int firstValue = stoi(instructionToExecute[6]); //get int value in first register
		destination = stoi(instructionToExecute[4]); // get the destination
		if (specificInstruction.compare(instructionType[1]) == 0) // if add
			result = firstValue + secondValue; // add the values
		else if (specificInstruction.compare(instructionType[3]) == 0) // if sub
			result = firstValue - secondValue; // subtract the values	
	}
	// I type instructions
	else if (typeOfInstruction.compare("I") == 0) {
		int immediate = stoi(instructionToExecute[2]); // get the immediate value
		int source = stoi(instructionToExecute[5]); // get the source value
		destination = stoi(instructionToExecute[4]); // get the destination
		if (specificInstruction.compare(instructionType[0]) == 0) // if addi
			result = immediate + source; // add immediate and source
		else if (specificInstruction.compare(instructionType[2]) == 0) // if subi
			result = source - immediate; // sutract immediate from source
	}
	// D type instructions
	else if (typeOfInstruction.compare("D") == 0) {
		if (specificInstruction.compare(instructionType[5]) == 0) {// if ldur
			result = stoi(instructionToExecute[8]); //value in address to load
			destination = stoi(instructionToExecute[4]); // register to store data memory value in
		}
		else if (specificInstruction.compare(instructionType[6]) == 0) { // if stur
			writeToReg = false; // don't write to registers
			result = stoi(instructionToExecute[7]); // value in register to store
			destination = stoi(instructionToExecute[5]); // address in data memory to store value in
			writeDataMem(destination, result); // write to dataMemory instead
		}
			
	}
	// C type instructions
	else if (typeOfInstruction.compare("C") == 0) {
		if (specificInstruction.compare(instructionType[7]) == 0) { // if CBZ
			int value = stoi(instructionToExecute[4]); // get the value to check
			if (value == 0) { // if it equals zero
				pcAddress = stoi(instructionToExecute[3]); // update the pcAddress
				cycles--;
			}
		}
		writeToReg = false; // don't write to registers
	}
	// B type instructions
	else if (typeOfInstruction.compare("B") == 0) {
		writeToReg = false; // don't write to registers
		pcAddress = stoi(instructionToExecute[2]); // update the pcAddress
	}

	// P type instructions
	else if (typeOfInstruction.compare("P") == 0) {
		writeToReg = false; // don't write to registers
		destination = stoi(instructionToExecute[2]); // which dataMemory address to write to
		result = stoi(instructionToExecute[3]); // value to write to dataMemory
		writeDataMem(destination, result); // write to dataMemory instead
	}

	if (writeToReg) { // if we need to write to dataMemory
		writeReg(destination, result);
	} // write to dataMemory
	cycles++;
	return pcAddress; // return pcAddress to check if we should branch
}

bool checkHazards(vector<string> current, vector<string> next) {
	string currentType = current[0];
	string nextType = next[0];
	bool bubble = false;

	if (currentType.compare("I") == 0 || currentType.compare("R") == 0) {
		if (nextType.compare("I") == 0 || nextType.compare("R") == 0) {
			//if (current[4].compare(next[2]) == 0 || current[4].compare(next[3]) == 0)
				//cout << "Forwarding @ instruction: " << PC + 1;
		}
	}

	if (currentType.compare("D") == 0) {
		if (nextType.compare("I") == 0 || nextType.compare("R") == 0) {
			if (current[4].compare(next[2]) == 0 || current[4].compare(next[3]) == 0) {
				//cout << "Forwarding @ instruction: " << PC + 1;
				bubble = true;
			}
		}
	}

	if (currentType.compare("I") == 0 || currentType.compare("R") == 0) {
		if (nextType.compare("D") == 0) {
			if (current[4].compare(next[3]) == 0 || current[4].compare(next[4]) == 0) {
				//cout << "Forwarding @ instruction: " << PC + 1;
				//if (!previousBubble)
					//bubble = true;
			}
				
		}
	}

	if (currentType.compare("I") == 0 || currentType.compare("R") == 0) {
		if (nextType.compare("C") == 0) {
				//cout << "Forwarding @ instruction: " << PC + 1;
		}
	}

	if (previousBubble) {
		cycles++;
		previousBubble = false;
	}

	if (PC > 1) {
		string previousType = decodedPreviousInstruction[0];
		if (previousType.compare("C") == 0 || previousType.compare("B") == 0) {
			bubble = true;
			previousBubble = true;
		}

	}






	return bubble;
}


// reset all necessary values
void reset() {
	// initialize reg and dataMem with 0's
	for (int i = 0; i < 32; i++) {
		reg[i] = 0;
	}
	for (int j = 0; j < 8; j++) {
		dataMem[j] = 0;
	}
	// reset PC, branch, cycles, decoded and current instructions
	PC = 0;
	branch = 0;
	cycles = 3;
	previousBubble = false;
	instructionMem.clear();
	decodedInstruction.clear();
	currentInstruction.clear();
	nextInstruction.clear();
	decodedNextInstruction.clear();
}

// print instruction memory, values in the register files, and data memory
void print() {
	
	cout  << "\t" << "Instruction Memory:" << "\n";
	if (instructionMem.size() == 0)
		cout << "\t" << "\t" << "(null)" << "\n";
	for (int i = 0; i < instructionMem.size(); i++) { //print instruction memory
		cout << "\t" << "\t" << "Instruction Memory[" << i << "] = " << instructionMem[i] << "\n";
	}
	cout << "\t" << "Register Files:" << "\n";
	for (int i = 0; i < 32; i++) { //print register files
		cout << "\t" << "\t" << "Register[" << i << "] = " << reg[i] << "\n";
	}
	cout << "\t" << "Data Memory:" << "\n";
	for (int i = 0; i < 8; i++) { //print data memory
		cout << "\t" << "\t" << "Data Memory[" << i << "] = " << dataMem[i] << "\n";
	}

}

// Drives simulation
int main()
{
	// (int i = 0; i < 3; i++)
	for (int i = 0; i < 3; i++) {
		// print instruction memory, values in the register files, and data memory before simulation 
		//cout << "\n" << "Before Input File: " << i + 1 << "\n";
		//print();
		// read instructions from file into instruction memory
		if (i == 0) {
			ifstream input("input1.txt");
			string data; // hold the read data
			if (input.good()) {
				while (getline(input, data)) //while there are lines to read, read them
				{
					instructionMem.push_back(data);		//store line
				}
			}
			input.close();
		}
		else if (i == 1) {
			ifstream input("input2.txt");
			string data; // hold the read data
			if (input.good()) {
				while (getline(input, data)) //while there are lines to read, read them
				{
					instructionMem.push_back(data);		//store line
				}
			}
			input.close();
		}
		else {
			ifstream input("input3.txt");
			string data; // hold the read data
			if (input.good()) {
				while (getline(input, data)) //while there are lines to read, read them
				{
					instructionMem.push_back(data);		//store line
				}
			}
			input.close();
		}
		int count = 1;
		//While there are instructions to process
		while (PC < instructionMem.size()) {
			//get the next instruciton
			currentInstruction = instructionMem[PC];
			if (PC + 1 < instructionMem.size())
				nextInstruction = instructionMem[PC + 1];
			//decode the instruction and read registers
			decodedInstruction = decodeInstruction(currentInstruction);
			if (PC + 1 < instructionMem.size()) {
				decodedNextInstruction = decodeInstruction(nextInstruction);
				if (checkHazards(decodedInstruction, decodedNextInstruction)) {
					cycles++;
				}

			}
			//ALU performs the operation, returns an int value branch
			branch = ALU(decodedInstruction);
			count++;
			// if branch is negative, add to current PC to jump back branch number of lines
			if (branch < 0) {
				PC = PC + branch;
				//PC++;
			}
			// if branch is positive, subtract from the current PC to jump forward branch number of lines
			else if (branch > 0) {
				PC = PC - branch;
				PC--;
			}
			// if branch == 0, continue to the next instruction
			else if (branch == 0)
				PC++;

			previousInstruction = currentInstruction;
			decodedPreviousInstruction = decodeInstruction(previousInstruction);
		}
		// print instruction memory, values in the register files, and data memory after simulation 
		cout << "\t" << "\n" << "After Input File: " << i + 1 << "\n";
		float CPI = ((float)cycles / (float)instructionMem.size());
		cout << "CPI =  " << cycles << " / " << instructionMem.size() << " = " << fixed << setprecision(2) << CPI << "\n";
		print();
		reset();
		
	}
}

