#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

int COMPARE_ASCII_FILES(const string& f1,const string& f2)
{
	fstream file1(f1.c_str()), file2(f2.c_str());
	char string1[16*256], string2[16*256];
	int j; j = 0;
	puts("Run comparison");
	while((!file1.eof())&&(!file2.eof()))
	{
		file1.getline(string1,16*256);
		file2.getline(string2,16*256);
		j++;
		if(strcmp(string1,string2) != 0)
		{
			cout << j << "-th strings are not equal" << "\n";
			cout << "   " << string1 << "\n";
			cout << "   " << string2 << "\n";
		return 1;
		}
	}
        return 0;
}










