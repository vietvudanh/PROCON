#include <iostream>
#include "e_collect.cpp"
#include <sstream>
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

void cpArr1Arr2(int *arr1, int *arr2, int size) {
	for(int i = 0; i < size; i++) {
		arr2[i] = arr1[i];
	}
}

// Cap nhat danh sach cum vua tao vao mat
void upgradeMat(Node mat[SIZE][SIZE], char * src, int begin, int end, int index) {
	int j = 0;
	int size = end - begin + 1;
	for(; j < size; j++) {
		mat[SIZE-1][index].str[j] = src[begin + j];
	}
	mat[SIZE - 1][index].str[j] = '\0';
}

string encodePhrase(Node en_table[SIZE][SIZE], char *src_string, int begin, int end) {
	string encode_str = "";
	for(int i = begin; i <= end; i++) {
		for(int j_out = 0; j_out < SIZE - 4; j_out++) {
			for(int j_in = 0; j_in < SIZE; j_in ++) {
				if(src_string[i] == en_table[j_out][j_in].str[0]) {
					stringstream ss;
					ss << j_out;
					encode_str += ss.str() + ' ';
					ss.str("");
					ss << j_in;
					encode_str += ss.str() + ' ';
					ss.str("");
				}
			}
		}	
	}
	return encode_str;
}

/*Tra ve vi tri xau lap lai trong bang anh xa, tu vi t*/
int posOfRepString(Node en_table[SIZE][SIZE], char *src_string, int begin, int end) {
	int index = 0;
	int str_len_1 = strlen(en_table[SIZE - 1][index].str);
	int str_len_2 = end - begin + 1;
	for(; index < SIZE;) {
		if(str_len_1 == str_len_2) {
			int i = 0;
			for(; i < str_len_1; i ++) {
				if(en_table[SIZE - 1][index].str[i] != src_string[i + begin]) {
					break;
				}
			}
			//cout << "i_in : " << i << " str_len_1: " << str_len_1 << endl;
			if(i == str_len_1) return index;
		}
		index++;
		str_len_1 = strlen(en_table[SIZE - 1][index].str);
	}
	return -1;	
}

string createEncode(Node en_table[SIZE][SIZE], char *src_string, int *main_arr, int str_length) {	
	// Bat dau ma hoa
	//cout << "Z: "; showArr(main_arr, str_length);
	int index = 0;
	string encode_str = "";
	for(int i = 0; i < str_length;) {
		if(0 != main_arr[i]) {
			int pos = posOfRepString(en_table, src_string, i, i + main_arr[i] - 1);
			//cout << "Pos: " << pos << endl;
			if(pos >= 0) {
				//cout << "Pos: " << pos << endl;
				// encode rep phrase
				encode_str += "9 ";
				stringstream ss;
				ss << pos;
				encode_str += ss.str() + ' ';
				ss.str("");
				i = i + main_arr[i];
			} else {
				encode_str += "10 " + encodePhrase(en_table, src_string, i, i + main_arr[i] - 1) + "10 ";
				upgradeMat(en_table, src_string, i, i + main_arr[i] - 1, index);
				index++;
				i = i + main_arr[i];
				}
			//cout << encode_str << endl;
		} else {
			// encode phrase
			int begin = i;
			while(0 == main_arr[i]) {
				i++;
				if(i == str_length) break;
			}
			encode_str += encodePhrase(en_table, src_string, begin, i - 1);
		}
	}	
	return encode_str;
}	

void initMat(Node en_table[SIZE][SIZE], char matrix[SIZE][SIZE]) {				  
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			en_table[i][j].str[0] = matrix[i][j];
		}
	}
}

void readFile(char *name, char *input_str) {
	ifstream my_read_file;
	my_read_file.open(name, ios :: in);
	
	if(my_read_file.is_open()) {
		while(!my_read_file.eof()) {
			my_read_file >> input_str;
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
	char *src_string = new char[ARRAY_SIZE];
	readFile(argv[1], src_string);
	int str_length = strlen(src_string);
	cout << "Length: " << str_length << endl;
	int main_arr[str_length];
	int clone_of_main_arr[str_length];	
	
	// TAO VA CAP NHAT CAC GIA TRI CHO BANG ANH XA
	// Khoi tao mang clone_of_main_arr va main_arr cac gia tri = 0
	initArr(main_arr, str_length);
	initArr(clone_of_main_arr, str_length);
	
	filterPhrase(src_string, main_arr, str_length);
	
	cpArr1Arr2(main_arr, clone_of_main_arr, str_length);
	filterSinglePhrase(src_string, clone_of_main_arr, str_length);
	showPhrase(src_string, clone_of_main_arr, str_length);

	Node en_table[SIZE][SIZE];
	Node list_repeat_string[SIZE_OF_REP];
	
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
					  
	initMat(en_table, matrix);	
		
	cout << "\n-SRC_STRING-" << endl;
	cout << src_string << endl;
	//cout << endl << "-MAT-" << endl;
	
	// ENCODE
	string encode_str = createEncode(en_table, src_string, main_arr, str_length);
	cout << encode_str << endl;
	writeFile(argv[2], encode_str);
	//showMat(en_table, SIZE, SIZE);
	//cin.ignore(80, '\n');
//	cin.get();
	return 0;
}
