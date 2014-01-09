// PlaySound.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#include <windows.h>
#include <sstream>
void readOutLoud(int i);
void readOutLoud(int i){
	//Ham nay buoc viet the nay, khong tu dong hoa duoc
	switch (i){
		case 1:PlaySound(TEXT("sound\\1.wav"), NULL, SND_FILENAME);break;
		case 2:PlaySound(TEXT("sound\\2.wav"), NULL, SND_FILENAME);break;
		case 3:PlaySound(TEXT("sound\\3.wav"), NULL, SND_FILENAME);break;
		case 4:PlaySound(TEXT("sound\\4.wav"), NULL, SND_FILENAME);break;
		case 5:PlaySound(TEXT("sound\\5.wav"), NULL, SND_FILENAME);break;
		case 6:PlaySound(TEXT("sound\\6.wav"), NULL, SND_FILENAME);break;
		case 7:PlaySound(TEXT("sound\\1s.wav"), NULL, SND_FILENAME);break;
		case 8:PlaySound(TEXT("sound\\2s.wav"), NULL, SND_FILENAME);break;
		case 9:PlaySound(TEXT("sound\\3s.wav"), NULL, SND_FILENAME);break;
		case 10:PlaySound(TEXT("sound\\4s.wav"), NULL, SND_FILENAME);break;
		case 11:PlaySound(TEXT("sound\\5s.wav"), NULL, SND_FILENAME);break;
		case 12:PlaySound(TEXT("sound\\6s.wav"), NULL, SND_FILENAME);break;
		
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i=1;i<=12;i++)
		readOutLoud(i);
	return 0;
}


