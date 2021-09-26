#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <bits/stdc++.h>
using namespace std;

hashTable loadDictWords(string &dictionaryfile)
{
    ifstream input(dictionaryfile);
    string word;
    //create hashtable and add dictionary entries
    hashTable dictionary = hashTable(100000);
    input.clear();
    //put pointer to first line
    input.seekg(0);
    while (getline(input, word))
    {
        //convert word in dictionary to be lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.insert(word);
    }
    return dictionary;
}

void validLine(string &line, int lineNum, hashTable &dictionary, ofstream &output)
{
    //convert line in textfile to be analyzed to lowercase
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    //initialize word to be tracked
    line.append(" ");
    string tword = "";
    bool number = false;
    //for loop to iterate through the line while spellchecking
    for (int i = 0; i < line.length(); i++)
    {
        //if index i is a letter from a-z or an apostrophe or a dash, add it to the word
        if ((line[i] >= 97 && line[i] <= 122 || (line[i] == 45 || (line[i] == 39))))
        {
            tword += line[i];
        }
            //if index i is a number, add it to the word and take note that it contains a number with a bool value
        else if (line[i] >= 48 && line[i] <= 57)
        {
            tword += line[i];
            number = true;
        }
        else
        {
            //words cannot be larger than 20 characters
            if (tword.length() > 20)
            {
                output << "Long word at line " << to_string(lineNum) << ", starts: " << tword.substr(0, 20) << endl;
            }
            //if the word doesn't contain a number and doesn't exist in the dictionary, it is an unknown word
            else if (number == false && dictionary.contains(tword) == false && tword != "")
            {
                output << "Unknown word at line " << to_string(lineNum) << ": " << tword << endl;
            }
            //reset values for word and number to check the next line
            tword = "";
            number = false;
        }
    }
}

//checks the input file, outputs errors to the output file
void checkInput(string &inputfile, string &outputfile, hashTable &dictionary)
{
    //open input and output streams
    ifstream input(inputfile);
    ofstream output(outputfile);
    //check through input file starting at line 1
    string l;
    string out;
    int lineNum = 1;
    while ( getline(input, l))
    {
        validLine(l, lineNum, dictionary, output);
        lineNum++;
    }
}

int main()
{
    clock_t startDict, endDict, startCheck, endCheck;
    string dictionaryfile;
    string inputfile;
    string outputfile;
    //Prompt the user for the dictionary, input, and output files
    cout << "Enter Dictionary File: ";
    cin >> dictionaryfile;
    //track time for dictionary to be loaded
    startDict = clock();
    hashTable dictionary = loadDictWords(dictionaryfile);
    endDict = clock();
    cout << "Total time for the dictionary to be loaded into the hash table: " << ((double)(endDict - startDict)) / CLOCKS_PER_SEC << "seconds" << endl;
    cout << "Enter Input File: ";
    cin >> inputfile;
    cout << "Enter Output File: ";
    cin >> outputfile;
    //track time for how long it takes to spellcheck input file
    startCheck = clock();
    checkInput(inputfile, outputfile, dictionary);
    endCheck = clock();
    cout << "Total time to spell check input file: " << ((double)(endCheck - startCheck)) / CLOCKS_PER_SEC << "seconds" << endl;
}