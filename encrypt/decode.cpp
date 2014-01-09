#include <iostream>
#include "e_collect.cpp"
#include <sstream>
#include <stdlib.h>
#include <fstream>

using namespace std;
#define SIZE 12
#define ARRAY_SIZE 4000
#define SIZE_OF_REP 100

struct Node {
	char str[ARRAY_SIZE];
};

void showMat(Node mat[SIZE][SIZE], int col, int row) {
	cout << " |0|1|2|3|4|5|6|7|8|9|0|1" << endl;
	for(int i = 0; i < row; i++) {
		cout << i % 10;
		for(int j = 0; j < col; j++) {
			//cout << strlen(mat[i][j].str) << "|";
			cout << "|" << mat[i][j].str;
		}
		cout << endl;
	}
}

string posToString(Node mat[SIZE][SIZE], int *encode_arr, int begin, int end) {
	string temp_str = "";
	for(int i = begin; i < end; i += 2) {
		temp_str.push_back(mat[encode_arr[i]][encode_arr[i+1]].str[0]);
	}
	return temp_str;
}

// Cap nhat danh sach cum vua tao vao mat
void upgradeMat(Node mat[SIZE][SIZE], string src, int index) {
	int j = 0;
	int size = src.size();
	for(; j < size; j++) {
		mat[SIZE-1][index].str[j] = src.at(j);
	}
	mat[SIZE - 1][index].str[j] = '\0';
}

string decode(Node mat[SIZE][SIZE], int *encode_arr, int size) {
	string decode_str = "";
	int index = 0;
	for(int i = 0; i < size;) {
		if(10 == encode_arr[i]) {
			int k = 0;
			int i_in = i + 1;
			int j = i_in;
			while(encode_arr[j] != 10) {
				j += 2;
			}
			string temp = posToString(mat, encode_arr, i_in, j - 1);
			decode_str += temp;
			upgradeMat(mat, temp, index);
			index++;
			showMat(mat, 12, 12);
			i = j + 1;
		//	cout << "Ele: " << encode_arr[i] << endl;
		//	cout << "Pos: " << i << endl;
			continue;
			}
		if(9 == encode_arr[i]) {
		//	cout << "Rep" << endl;
			decode_str += mat[SIZE - 1][encode_arr[i + 1]].str;
			i += 2;
		} else {
		//	cout << "Other" << endl;
			decode_str += mat[encode_arr[i]][encode_arr[i+1]].str[0];
			i += 2;
		}
	}
	return decode_str;
}

// Khoi tao bang anh xa
void initEnTable(Node en_table[SIZE][SIZE], char matrix[SIZE][SIZE]) {
		for(int i = 0; i < SIZE; i++) {
			for(int j = 0; j < SIZE; j++) {
				en_table[i][j].str[0] = matrix[i][j];
			}
		}
}

// Tao mang int toa do
void createEncodeArr(string encode, int *encode_arr, int &size) {
	char *encode_char = new char [encode.size() + 1];
	strcpy(encode_char, encode.c_str());
	
	char *pch;
	int index = 0;
	
	pch = strtok(encode_char, " ");
	while(pch != NULL) {
		encode_arr[index] = atoi(pch);
		pch = strtok(NULL, " ");
		index++;
	}
	size = index;
}

void readFile(char *name, string &input_str) {
	ifstream my_read_file;
	my_read_file.open(name, ios :: in);
	
	if(my_read_file.is_open()) {
		while(!my_read_file.eof()) {
			getline(my_read_file, input_str);
		} 
	}
	my_read_file.close();
	return;
}

void writeFile(char *name, string output_str) {
	ofstream myfile;
	myfile.open(name, ios :: out);
	myfile << output_str;
	myfile.close();
}

// Tham so moi truong: Ten file input.txt output.txt
int main(int argc, char *argv[]) {
	
	string encode = "";
	char arr[ARRAY_SIZE];
	readFile(argv[1], encode);
	//cout << encode << endl;
	// Bang anh xa
	Node en_table[SIZE][SIZE];
	
	int size = encode.size();
	int encode_arr[size];
		
	createEncodeArr(encode, encode_arr, size);
	//cout << "-ENCODE_STR-" << endl;
	//showArr(encode_arr,size);
	//cout << endl;
	
	char matrix[SIZE][SIZE] = {
						{'!', '1', 'A', 'Q', 'a', 'q', '0', '@', 'P', '`', 'p', 'o'},
						{'"', '2', 'B', 'R', 'b', 'r', '#', '3', 'C', 'S', 'c', 's'},
						{'$', '4', 'D', 'T', 'd', 't', '%', '5', 'E', 'U', 'e', 'u'},
						{'&', '6', 'F', 'V', 'f', 'v', '\'', '7', 'G', 'W', 'g', 'w'},
						{'(', '8', 'H', 'X', 'h', 'x', ')', '9', 'I', 'Y', 'i', 'y'},
						{'*', ':', 'J', 'Z', 'j', 'z', '+', ';', 'K', 'k', ',', '<'},
						{'L', 'l', '-', '=', 'M', 'm', '.', '>', 'N', 'n', '/', '?'},
						{'O', '_', '”', '’'}						   							   
					  };
					  
	// Khoi tao bang anh xa
	initEnTable(en_table, matrix);
	
	// Cap nhat cac xau lap vao bang anh xa
	string decode_str = decode(en_table, encode_arr, size);
	
	//showMat(en_table, SIZE, SIZE);
	writeFile(argv[2], decode_str);
	//cin.ignore(80, '\n');
//	cin.get();
	return 0;
}
