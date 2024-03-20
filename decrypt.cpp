#include <iostream>
#include <bitset>
#include <string>
#include <cmath>
using namespace std;

bitset<64> key;
bitset<48> subkey[16];


int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2,
			 60, 52, 44, 36, 28, 20, 12, 4,
			 62, 54, 46, 38, 30, 22, 14, 6,
			 64, 56, 48, 40, 32, 24, 16, 8,
			 57, 49, 41, 33, 25, 17, 9,  1,
			 59, 51, 43, 35, 27, 19, 11, 3,
			 61, 53, 45, 37, 29, 21, 13, 5,
			 63, 55, 47, 39, 31, 23, 15, 7};

int FP[] = { 40, 8, 48, 16, 56, 24, 64, 32,
			 39, 7, 47, 15, 55, 23, 63, 31,
			 38, 6, 46, 14, 54, 22, 62, 30,
			 37, 5, 45, 13, 53, 21, 61, 29,
			 36, 4, 44, 12, 52, 20, 60, 28,
			 35, 3, 43, 11, 51, 19, 59, 27,
			 34, 2, 42, 10, 50, 18, 58, 26,
			 33, 1, 41,  9, 49, 17, 57, 25 };

int key64_To_56[] = { 57, 49, 41, 33, 25, 17,  9,
			           1, 58, 50, 42, 34, 26, 18,
			          10,  2, 59, 51, 43, 35, 27,
		        	  19, 11,  3, 60, 52, 44, 36,
			          63, 55, 47, 39, 31, 23, 15,
			           7, 62, 54, 46, 38, 30, 22,
			          14,  6, 61, 53, 45, 37, 29,
			          21, 13,  5, 28, 20, 12,  4 };

int key56_To_48[] = { 14, 17, 11, 24,  1,  5,
			           3, 28, 15,  6, 21, 10,
			          23, 19, 12,  4, 26,  8,
					  16,  7, 27, 20, 13,  2,
					  41, 52, 31, 37, 47, 55,
					  30, 40, 51, 45, 33, 48,
					  44, 49, 39, 56, 34, 53,
					  46, 42, 50, 36, 29, 32 };

int shift_per_round_forkey[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int Expend[] = { 32,  1,  2,  3,  4,  5,
				  4,  5,  6,  7,  8,  9,
				  8,  9, 10, 11, 12, 13,
				 12, 13, 14, 15, 16, 17,
				 16, 17, 18, 19, 20, 21,
				 20, 21, 22, 23, 24, 25,
				 24, 25, 26, 27, 28, 29,
				 28, 29, 30, 31, 32,  1 };

int S_Box[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};

int Permutation[] = { 16,  7, 20, 21,
					  29, 12, 28, 17,
				       1, 15, 23, 26,
			           5, 18, 31, 10,
			           2,  8, 24, 14,
		              32, 27,  3,  9,
		              19, 13, 30,  6,
		              22, 11,  4, 25 };

bitset<32> function_f(bitset<32> R, bitset<48> key)
{
 	bitset<48> expand_R;
	for (int i = 0; i < 48; i++) {
		expand_R[47 - i] = R[31 - (Expend[i] - 1)];
	}
	//cout << expand_R << endl;
	expand_R = expand_R ^ key;
	bitset<32> output_32bit;
	int index = 0;
	int count = 47;
	for (int i = 0; i < 48; i = i + 6){
		int row = expand_R[i] + expand_R[i + 5] * 2;
		int col = expand_R[i + 1] + expand_R[i + 2] * 2 + expand_R[i + 3] * 4 + expand_R[i + 4] * 8;
		int S_Box_value = S_Box[count / 6][row][col]; 
		bitset<4> binary(S_Box_value);
		output_32bit[index] = binary[0];
		output_32bit[index + 1] = binary[1];
		output_32bit[index + 2] = binary[2];
		output_32bit[index + 3] = binary[3];
		count = count - 6;
		index += 4;
	}
	//cout << output_32bit<<endl;
	bitset<32> P_switch = output_32bit;
	for (int i = 0; i < 32; i++) {
		//cout << Permutation[i] << endl;
		output_32bit[31 - i] = P_switch[31 - (Permutation[i] - 1)];
	}
	
	return output_32bit;
}


bitset<28> leftShift(bitset<28> key, int shift)
{
	int temp;
	bitset<28> tmp = key;
	if (shift == 1) {
		temp = key[27];
		bitset<28> zero(temp);
		key = key << 1;
		key = key ^ zero;
	}
	else if (shift == 2) {
		temp = key[27] * 2 + key[26];
		bitset<28> zero(temp);
		key = key << 2;
		key = key ^ zero;
	}
	return key;
}

void generateKey(){
	bitset<56> realKey;
	bitset<28> left;
	bitset<28> right;
	bitset<48> compressKey;
	for (int i = 0; i < 56; ++i) {
		realKey[55 - i] = key[63 - (key64_To_56[i] - 1)];
	}
	//cout << realKey << endl << endl << endl;
	for (int round = 0; round < 16; round++){
		for (int i = 0; i <28; i++) {
			right[i] = realKey[i];
		}
		for (int i = 0; i < 28; i++) {
			left[i] = realKey[28 + i];
		}

		left = leftShift(left, shift_per_round_forkey[round]);
		right = leftShift(right, shift_per_round_forkey[round]);
		//cout << left << endl << right << endl << endl;
		for (int i = 0; i < 28; i++) {
			realKey[i + 28] = left[i];
		}
		for (int i = 0; i < 28; i++) {
			realKey[i] = right[i];
		}
		//cout << realKey << endl << left << right <<endl;
		for (int i = 0; i < 48; i++) {
			compressKey[47 - i] = realKey[55 - (key56_To_48[i] - 1)];
		}
		subkey[round] = compressKey;
		cout << compressKey << endl;
		cout << endl;
	}
}


bitset<64> stringToBitset(string s){
	string output;
	for (int i = 0; i < 16; i++) {
		switch (s[i]) {
			case '0':
				output += "0000";
				break;
			case '1':
				output += "0001";
				break;
			case '2':
				output += "0010";
				break;
			case '3':
				output += "0011";
				break;
			case '4':
				output += "0100";
				break;
			case '5':
				output += "0101";
				break;
			case '6':
				output += "0110";
				break;
			case '7':
				output += "0111";
				break;
			case '8':
				output += "1000";
				break;
			case '9':
				output += "1001";
				break;
			case 'A':
				output += "1010";
				break;
			case 'B':
				output += "1011";
				break;
			case 'C':
				output += "1100";
				break;
			case 'D':
				output += "1101";
				break;
			case 'E':
				output += "1110";
				break;
			case 'F':
				output += "1111";
				break;
		}
	}
	bitset<64> bits(output);
	return bits;
}

string bitsetTostring(bitset<64> bits) {
	string temp;
	string output;
	int c = 0;
	int count = 3;
	temp = bits.to_string();
	for (int i = 0; i < 64; i = i + 4) {
		for (int j = 0; j < 4; j++) {
			c += ((temp[i + j] - '0') * pow(2, count));
			count--;
		}
		if (c > 9) {
			output += char(c + 55);
		}
		else {
			output += char(c + 48);
		}
		c = 0;
		count = 3;
	}
	return output;
}

bitset<64> decrypt(bitset<64>& cipher)
{
	bitset<64> plain;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	for (int i = 0; i < 64; i++) {
		currentBits[63 - i] = cipher[63 - (IP[i] - 1)];
 	}
	//cout << currentBits << endl;
	for (int i = 0; i < 32; i++) {
		right[i] = currentBits[i + 32];
	}
	for (int i = 0; i < 32; i++) {
		left[i] = currentBits[i];
	}
	cout << left << endl << right << endl;
	cout << endl;
	for (int round = 0; round < 16; round++){
		newLeft = right ^ function_f(left, subkey[15 - round]);
		right = left;
		left = newLeft;
		cout << function_f(left, subkey[15 - round]) << endl;
		cout << left << endl << right << endl;
		cout << endl;
	}
	
	for (int i = 0; i < 32; i++) {
		plain[i + 32] = left[i];
	}
	for (int i = 0; i < 32; i++) {
		plain[i] = right[i];
	}
	currentBits = plain;
	for (int i = 0; i < 64; i++) {
		plain[63 - i] = currentBits[63 - (FP[i] - 1)];
	}
	return plain;
}
int main(int argc, char** argv){
	string plaintext;
	string input_key;
	string input;
	bitset<64> cipher;
	bitset<64> plain;
	//bitset<32> R("11110000101010101111000010101010");
	//bitset<48> k1("000110110000001011101111111111000111000001110010");
	// << function_f(R, k1) << endl;
	input = argv[2];
	input_key = argv[4];
	/*cin >> input >> input_key;*/
	input = input.erase(0, 2);
	input_key = input_key.erase(0, 2);
	cipher = stringToBitset(input);
	key = stringToBitset(input_key);
	generateKey();
	plain = decrypt(cipher);
	plaintext = bitsetTostring(plain);
	cout << "0x" << plaintext << endl;
}
