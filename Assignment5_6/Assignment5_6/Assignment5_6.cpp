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


int main()
{

	// create register file
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

	// format instructions into program storage

	// create PC 
	int PC = 0;
	// initialize instruction memory
	int mem[0x1000];
	// load memory with all 0
	for (int i = 0; i < 0x1000; i++) {
		mem[i] = 0;
	}



}

