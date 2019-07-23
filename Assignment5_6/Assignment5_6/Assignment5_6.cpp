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

//vector of possible instruction types
vector<string> instructionType = {"ADDI","ADD", "SUBI", "SUB", "DMEM", "LDUR", "STUR", "CBZ", "B"};

vector<string> decodeInstruction (string instruction) {
	// Find position of ':' using find() 
	int space = instruction.find(" ");
	int comma = instruction.find_first_of(",");
	// Copy substring after pos 
	string type = instruction.substr(0, space);
	vector<string> decoded;
	
	// Add instruction type as first entry in decoded instruciton vector
	for (int i = 0; i < instructionType.size(); i++) {
		if (type.compare(instructionType[i]) == 0) {

			decoded.push_back(instructionType[i]);
			switch (i) {
			case 0: // ADDI
				string start = instruction.substr(space + 1, comma);
				break;
			case 1: // ADD
				break;
			case 2: // SUBI
				break;
			case 3: // SUB
				break;
			case 4: // DMEM
				break;
			case 5: // LDUR
				break;
			case 6: // STUR
				break;
			case 7: // CBZ
				break;
			case 8: // B
				break;
			}
		}
	}

}


void ALU() {
	/*
	
	switch(instruction)
	case ADD
	case ADDI
	case SUB
	case SUBI
	case DMEM
	case LDUR
	case STUR
	case CBZ
	case B
	
	
	
	*/

}

void simPrint() {

}


int main()
{

	// create and initialize register file
	int reg[32];
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

	// create PC 
	int PC = 0;
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

	//Fetch instruction
	for (int i = 0; i < instructionMem.size; i++) {
		string currentInstruction = instructionMem[i];
		vector<string> decodedInstruction = decodeInstruction(currentInstruction);
	}
}

