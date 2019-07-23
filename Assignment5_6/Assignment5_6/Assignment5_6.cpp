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

using namespace std;


// create and initialize register file
int reg[32];


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
	int regInt = stoi(regToRead);
	int value = reg[regInt];
	string regString = to_string(value);
	return regString;
}

void writeReg(int regID, int value) {
	reg[regID] = value;
}


//Decode instruction
vector<string> decodeInstruction (string instruction) {
	// Find position of ' ' using find() 
	int space = instruction.find(" ");
	// Copy the substring after instruction
	string type = instruction.substr(0, space);
	vector<string> decoded;
	string rawString;
	string getValue;
	vector<string> seperated;
	
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
			//@TODO
			else if (i == 5 || i == 6) { // if it is an D type
				decoded.push_back(decoded[1]); // address
				decoded.push_back(readReg(decoded[2])); // base
				decoded.push_back(readReg(decoded[3])); // dest / source
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


void ALU(vector<string> instructionToExecute) {
	string typeOfInstruction = instructionToExecute[0];
	string specificInstruction = instructionToExecute[1];
	int result = 0;
	int destination = 0;

	// R type instructions
	if (typeOfInstruction.compare("R") == 0) {
		int secondValue = stoi(instructionToExecute[5]);
		int firstValue = stoi(instructionToExecute[6]);
		destination = stoi(instructionToExecute[4]);
		if (specificInstruction.compare(instructionType[1]) == 0) // if add
			result = firstValue + secondValue;
		else if (specificInstruction.compare(instructionType[3]) == 0) // if sub
			result = firstValue - secondValue;			
	}
	// I type instructions
	else if (typeOfInstruction.compare("I") == 0) {
		int immediate = stoi(instructionToExecute[2]);
		int source = stoi(instructionToExecute[5]);
		destination = stoi(instructionToExecute[4]);
		if (specificInstruction.compare(instructionType[0]) == 0) // if addi
			result = immediate + source;
		else if (specificInstruction.compare(instructionType[2]) == 0) // if subi
			result = source - immediate;
	}
	// D type instructions
	else if (typeOfInstruction.compare("D") == 0) {

	}
	// C type instructions
	else if (typeOfInstruction.compare("C") == 0) {

	}
	// B type instructions
	else if (typeOfInstruction.compare("B") == 0) {

	}

	writeReg(destination, result);

}




int main()
{
	reg[0] = 0; // X0 Arguments/Results
	reg[1] = 0; // X1 Arguments/Results
	reg[2] = 0; // X2 Arguments/Results
	reg[3] = 0; // X3 Arguments/Results
	reg[4] = 0; // X4 Arguments/Results
	reg[5] = 0; // X5 Arguments/Results
	reg[6] = 0; // X6 Arguments/Results
	reg[7] = 0; // X7 Arguments/Results
	reg[8] = 0; // X8 indirect result location register
	reg[9] = 0; // X9 Temp
	reg[10] = 0; // X10 Temp
	reg[11] = 0; // X11 Temp
	reg[12] = 0; // X12 Temp
	reg[13] = 0; // X13 Temp
	reg[14] = 0; // X14 Temp
	reg[15] = 0; // X15 Temp
	reg[16] = 0; // X16 Temp
	reg[17] = 0; // X17 Temp
	reg[18] = 0; // X18 Temp
	reg[19] = 0; // X19 Saved
	reg[20] = 0; // X20 Saved
	reg[21] = 0; // X21 Saved
	reg[22] = 0; // X22 Saved
	reg[23] = 0; // X23 Saved
	reg[24] = 0; // X24 Saved
	reg[25] = 0; // X25 Saved
	reg[26] = 0; // X26 Saved
	reg[27] = 0; // X27 Saved
	reg[28] = 0; // X28 (SP), Stack Pointer
	reg[29] = 0; // X29 (FP), Frame Pointer
	reg[30] = 0; // X30 (LR), Link Register (return address)
	reg[31] = 0; // XZR Const 0

	// create PC and NPC 
	int PC = 0;
	int NPC = 0;
	// initialize instruction memory
	vector<string> instructionMem;
	//create and intialize data memory
	int dataMem[8] = { 0,0,0,0,0,0,0,0 };

	// read instructions from file into instruction memory
	ifstream input("input1.txt");
	string data;
	if (input.good())
	{
		while (getline(input, data)) //while there are lines to read, read them
		{
			instructionMem.push_back(data);		//store line
		}
	}
	input.close();
	printf("Simulation Begins \n");

	vector<string> decodedInstruction;

	//While there are instructions to process
	while(PC < instructionMem.size()) {
		//get the next instruciton
		string currentInstruction = instructionMem[PC];
		// Increment the PC and store the value in NPC
		//NPC = PC++;
		//decode the instruction and read registers
		decodedInstruction = decodeInstruction(currentInstruction);
		//ALU performs the operation
		ALU(decodedInstruction);
		//memory access/ branch completion
		//write back
		PC++;

	}

	cout << reg[11];
}

