#include <iostream>
#include <boost/algorithm/string.hpp>
#include <cstring>
#include <iterator>
#include <numeric>
#include "hashtable.h"
using namespace std;
using namespace cop4530;

void menu();
void zeroparam();
string parser(string sentence, HashTable<string> x);


int main (int argc, char* argv[])
{
	//if program is called with no parameters, call program that tests hash table
	if (argv[1] == 0)
		zeroparam();
	else
	{
		//else spell checker, create a hashtable, vector, and appropriate filestreams
		HashTable<string> table;
		vector<string> finalvec;
		table.load(argv[1]);
		ifstream ifile(argv[2]);
		string line;
		while (getline(ifile, line))
		{
			//iterate through checkfile, call parser, add corrected strings to a final vector
			finalvec.push_back(parser(line, table));
		}
		//output corrected strings to file, close files, spell check over
		ofstream ofile(argv[3]);
		for (int i = 0; i < finalvec.size(); i++)
			ofile << finalvec[i] << endl;
		ofile.close();
		ifile.close();

	}
	return 0;
}

void zeroparam()
{
	//pretty easy, calling member functions according to what is selected
	int size;
	char choice;
	string word;
	cout << "Enter preferred hash table capacity: ";
	cin >> size;
	HashTable<string> table(size);
	while (choice != 'x')
	{
		menu();
		cin >> choice;
		if (choice == 'l')
		{
			cout << "Enter dictionary filename to load from: ";
			cin >> word;
			const char *c = word.c_str();	
			if (table.load(c))
				cout << "Dictionary loaded successfully" << endl;
			else
				cout << "Cannot open file " << word << endl;
		}
		if (choice == 'a')
		{
			cout << "Enter word: ";
			cin >> word;
			cout << endl;
			if (table.insert(word))
				cout << "Word " << word << " added.";
		}
		if (choice == 'r')
		{
			cout << "Enter word: ";
			cin >> word;
			cout << endl;
			if (table.remove(word))
				cout << "Word " << word << " deleted" << endl;
			else
				cout << "Word not found. Could not delete" << endl;
		}
		if (choice == 'c')
		{ table.clear(); }
		if (choice == 'd')
		{
			if (table.getSize() == 0)
				cout << "Table is empty!" << endl;
			else
				table.dump();
		}
		if (choice == 'f')
		{
			cout << "Enter word: ";
			cin >> word;
			cout << endl;
			if (table.contains(word))
				cout << "Word " << word << " found" << endl;
			else
				cout << "Word " << word << " not found" << endl;
		}
		if (choice == 's')
		{
			cout << "Table size is " << table.getSize() << endl;
		}
		if (choice == 'w')
		{
			cout << "Enter dictionary file name to write to: ";
			cin >> word;
			const char *c = word.c_str();
			if (table.write_to_file(c))
				cout << "Written successfully" << endl;
			else
				cout << "Not written successfully" << endl;
		}
	}
}
string parser(string sentence, HashTable<string> x)
{
	//real work is done here, first, parse sentence into individual words, store in parent vector
	istringstream is (sentence);
	vector<string> parent {istream_iterator<string>{is}, istream_iterator<string>{}};
	vector<string> child;
	for (int i = 0; i < parent.size(); i++)
		//if a word is not in the hashtable, do stuff
		if (!x.contains(parent[i]))
		{
			string temp = parent[i];
			int count = 0;
			char letter = 'a';
			//iterate through the alphabet for each letter of the string 
			while (count != temp.length())
			{
				char c = temp[count];
				for (int j = 0; j < 26; j++)
				{
					temp[count] = letter;
					if (x.contains(temp))
					{
						//found a candidate, add to child vector
						child.push_back(temp);
					}
					//iterate to next letter in alphabet
					letter += 1;
				}
				//reset everything
				temp[count] = c;
				letter = 'a';
				count++;
			}
			//printing incorrect words to upper case
			for (int k = 0; k < parent.size(); k++)
			{
				if (k == i)
				{
					string temp2 = boost::to_upper_copy<string>(parent[i]);
					cout << temp2 << ' ';
				}
				else
					cout << parent[k] << ' ';
			}
			cout << endl;
			//formatting stuff
			cout << "===================================="	<< endl;
			for (int p = 0; p < child.size(); p++)
				cout << p << "): " << child[p] << endl;	
			cout << "-1 (no change):" << endl;
			cout << "===================================="	<< endl;
			int choice;
			cout << "Your choice: ";
			cin >> choice;
			//correct the word
			if (choice != -1)
				parent[i] = child[choice];
		}
		//append spaces to the end of each string of parent, accumulate to a finalstring, return said finalstring
		for (int i = 0; i < parent.size(); i++)
			parent[i].append(" ");
		string finalstring;
		finalstring = accumulate(begin(parent), end(parent), finalstring);
		return finalstring;
			
}
void menu()
{
	cout << "\n\n";
	cout << "l - Load Dictionary From File" << endl;
	cout << "a - Add Word" << endl;
	cout << "r - Remove Word" << endl;
	cout << "c - Clear HashTable" << endl;
	cout << "f - Find Word" << endl;
	cout << "d - Dump HashTable" << endl;
	cout << "s - HashTable Size" << endl;
	cout << "w - Write to File" << endl;
	cout << "x - Exit program" << endl;
	cout << "\nEnter choice : ";
}
