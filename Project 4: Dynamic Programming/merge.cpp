#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

//A and B will be at most 1000 characters
//global matrix to store computed values (bottom up dynamic programming)
bool memory[1100][1100];

int main()
{
	//getting input and output file from user
	string inputFile, outputFile, A, B, C;
	ifstream readInput;
	ofstream readOutput;
	cout << "Enter name of input file: ";
	cin >> inputFile;
	cout << "Enter name of output file: ";
	cin >> outputFile;
	readInput.open(inputFile.c_str());
	readOutput.open(outputFile.c_str());
	while (readInput >> A && readInput >> B && readInput >> C) 
	{
		//C won't be a merge of A and B if the combined lengths don't equal each other
		if ((A.length() + B.length()) != C.length()) 
		{
			readOutput << "*** NOT A MERGE ***" << endl;
			continue;
		}
		//initializing column and row 0 to compute future values
		for (int i = 0; i < B.length()+1; i++) 
		{
			for (int j = 0; j < A.length()+1; j++) 
			{
				if (i == 0 && j > 0) 
				{
					if (C[j-1] == A[j-1]) 
					{
						//1st letter from A
						if(j == 1)
						{
							memory[i][j] = true;
							continue;
						}
						//letters can only come from A when i==0
						//square in matrix at i, j-1 must be true for i,j to be true
						else
						{
							memory[i][j] = memory[i][j-1];
							continue;
						}
					}
				}
				else if (j == 0 && i > 0) 
				{
					if (C[i-1] == B[i-1]) 
					{
						//same thing for B when j==0
						if(i == 1)
						{
							memory[i][j] = true;
							continue;
						}
						else
						{
							memory[i][j] = memory[i-1][j];
							continue;
						}
					}
				}
				memory[i][j] = false;
			}
		}
		//go through the rest of the matrix and update values appropriately using computed values
		for (int i = 1; i < B.length()+1; i++) 
		{
			for (int j = 1; j < A.length()+1; j++) 
			{
				memory[i][j] = false;
				if (memory[i-1][j] || memory[i][j-1]) 
				{
					if (memory[i-1][j]) 
					{
						if (C[i+j-1] == B[i-1]) 
						{
							memory[i][j] = true;
						}
					}
					else if (memory[i][j-1]) 
					{
						if (C[i+j-1] == A[j-1]) 
						{
							memory[i][j] = true;
						}
					}
				}
			}
		}
		//if C is a correct merge of B and A, (m,n) should be true
		if (memory[B.length()][A.length()]==1) 
		{
			int m = B.length();
			int n = A.length();
			while (n > 0 && m >= 0) 
			{
				//letters taken from A are capitalized for the output
				if (memory[m][n] && (m == 0 || !memory[m-1][n])) 
				{
					C[m+n-1] = toupper(C[m+n-1]);
					n--;
				}
				else
					m--;
			}
			readOutput << C << endl;
		}
		else
		{
			readOutput << "*** NOT A MERGE ***" << endl;
		}
	}
	readInput.close();
	readOutput.close();
	return 0;
}