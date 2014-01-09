#include <iostream>
#include <string.h>
using namespace std;

// Hien thi
void showArr(int arr[], int arr_size) {
	for(int i = 0; i < arr_size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

// initial an array
void initArr(int *arr, int arr_size) {
	for(int i = 0; i < arr_size; i++) {
		arr[i] = 0;
	}
}

// find max
int findMax(int *arr, int arr_size) {
	
	int max = -1;
	
	for(int i = 0; i < arr_size; i++) {
		if(max < arr[i]) {
			max = arr[i];
		}	
	}
	
	return max;
}

/* Don lay cac gia tri can thiet trong mang*/
void cleanArr(int *arr, int arr_size) {
	
	int max = findMax(arr, arr_size);
	int dem = 0;
	
	for(int i = 0; i < arr_size; i++) {
		if(arr[i] != max) arr[i] = 0;
		else dem++;
	}	
	
	if((max == 3 && dem < 3) || max < 3) {
		for(int i = 0; i < arr_size; i++) arr[i] = 0;
	}
}

/* Kiem tra co phan tu nao va cham nhau hay khong, co tra ve TRUE, nguoc lai: FALSE*/
bool checkClash(int *src_arr, int *det_arr, int arr_size) {
		for(int i = 0; i < arr_size; i++) {
			for(int j = i; j < src_arr[i] + i; j++) {
				if(det_arr[j] != 0) {
					return true;
				}
			}
			
			for(int j = i; j < det_arr[i] + i; j++) {
				if(src_arr[j] != 0) {
					return true;
				}
			}
		}
		
		return false;
}

/* Cap nhat lai mang luu gia tri vua thong ke duoc tren co so cac gia tri da
phan tich duoc truoc do*/
void upgradeArr(int *src_arr, int *det_arr, int arr_size) {
	int max_src = findMax(src_arr, arr_size);
	int max_det = findMax(det_arr, arr_size);
	bool check_clash = checkClash(src_arr, det_arr, arr_size);
	
	if(max_det >= max_src && false == check_clash) {
		for(int i = 0; i < arr_size; i++) 
					if(0 == src_arr[i] && 0 != det_arr[i])  src_arr[i] = det_arr[i];
	} else if(max_det > max_src) {
		for(int i = 0; i < arr_size; i++) src_arr[i] = det_arr[i];
	} else {
		if(false == check_clash) 
			for(int i = 0; i < arr_size; i++) 
				if(0 == src_arr[i] && 0 != det_arr[i])  src_arr[i] = det_arr[i];
	}
}

/* Create Z array, which stores elements*/
void filterPhrase(char *src_string, int *main_arr, int str_length) {
	for(int pos = 0; pos < str_length; pos ++) {
		int *z = new int [str_length];
		
		for(int i = 0; i < str_length; i++) {
			z[i] = 0;
		}
			
		int R = 0, L = 0;
		for(int i = 1 + pos; i < str_length; i++) {
			if(i > R) {
				L = R = i;
				while(R < str_length && src_string[R - L + pos] == src_string[R]) R++;
				z[i] = R - L; R--;
				//showArr(z, str_length);
			} else {
				int k = i - L;
				if(z[k] < i - R + 1) z[i] = z[k];
				else {
					L = i;
					while(R < str_length && src_string[R - L  + pos] == src_string[R]) R++;
					z[i] = R - L; R--;	
					//showArr(z, str_length);
				}
			}
		}
		for(int i = pos; i < str_length; i++) {
			if(z[i] > i - pos) {
				z[i] = 0;
			}
		}
		z[pos] = findMax(z, str_length);
		cleanArr(z, str_length);
		upgradeArr(main_arr, z, str_length);
	}
}

void filterSinglePhrase(char *src, int *main_arr, int str_length) {
	for(int i_out = 0; i_out < str_length; i_out++) {
		if(0 == main_arr[i_out]) continue;
		for(int i = i_out; i < str_length; i++) {
			if(0 != main_arr[i]) {
				int temp1 = main_arr[i];
				int i_temp1 = i;
				for(int j = i + temp1; j < str_length; j++) {
					if(main_arr[j] == temp1) {
						int L = j;
						int count = 0;
						for(int m = i_temp1; m < temp1 + i_temp1; m++) {
							if(src[m] == src[j]) count++;
							j++;
						}
						if(count == temp1) {
							for(int k = L; k < temp1 + L; k++) {
								main_arr[k] = 0;
							}
						}
					}
				}
			}
		}
	}
}

// Hien thi chuoi lap
void showPhrase(char* src_string, int *main_arr, int str_length) {
	for(int i = 0; i < str_length; i++) {
		for(int j = i; j < main_arr[i] + i; j++) {
			cout << src_string[j];
		}
		if(main_arr[i] != 0) cout << endl;
	}
}
