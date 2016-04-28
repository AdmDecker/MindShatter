/*
All purpose brainfuck interpreter
Copyright Adam Decker 2016
Version 1.0.0
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace std;
enum mode{file, commandline};


bool end_loop(int &i);
bool process_line(string line);
bool increment_ptr();
bool decrement_ptr();
bool increment_val();
bool decrement_val();
bool output_byte();
bool input_byte();
bool start_loop(int &i);
bool end_loop(int &i);
bool process_cl_args(int argc, char *argv[], string &infile, string &outfile, mode &input, mode &output);
bool preprocess_line(string line, int lineNumber);


char data[30000] = { '\0' };
int ptr = 0;
mode input = commandline;
mode output = commandline;
ofstream fout;
vector<int> loop_stack;


void show_usage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)> SOURCES"
		<< "Options:\n"
		<< "\t-i\t\tSpecify the input destination path\n"
		<< "\t-o,--destination DESTINATION\tSpecify the output destination path"
		<< std::endl;
}


int main(int argc, char *argv[])
{
	string infile = "\0";
	string outfile = "\0";

	if (!process_cl_args(argc, argv, infile, outfile, input, output))
		return 1;

	cout << "MindShatter v1.0" << endl;
	//Initialize brainfuck array

	bool dead = false;
	int lineNumber = 1;
	string line;
	
	while (!dead)
	{
		if (input == commandline)
		{
			cout << ">>> ";
			cin.clear();
			getline(cin, line);
			if(preprocess_line(line, lineNumber))
				process_line(line);
			lineNumber++;
		}
		else
		{
			ifstream fin(infile.c_str());
			if (!fin)
			{
				cerr << "Error opening input file\n";
				return 1;
			}

			while (!fin.eof())
			{
				cin.clear();
				getline(fin, line);
				if (preprocess_line(line, lineNumber))
					process_line(line);
				lineNumber++;
			}
			dead = true;
		}
	}
}


bool process_cl_args(int argc, char *argv[], string &infile, string &outfile, mode &input, mode &output)
{
	for (int i = 1; i < argc; ++i)
	{
		string arg = argv[i];
		if ((arg == "-i"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				infile = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
				input = file;
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--input file option requires one argument." << std::endl;
				return false;
			}
		}
		else if (arg == "-o")
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				outfile = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
				output = file;
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--output file option requires one argument." << std::endl;
				return false;
			}
		}
	}
	return true;
}

bool process_line(string line)
{
	int i = 0;
	int length = line.length();

	while(i < length)
	{
		switch (line.at(i))
		{
		case '>':
			if (!increment_ptr())
				return false;
			break;
		case '<':
			if(!decrement_ptr())
				return false;
			break;
		case '+':
			if(!increment_val())
				return false;
			break;
		case '-':
			if (!decrement_val())
				return false;
			break;
		case '.':
			if(!output_byte())
				return false;
			break;
		case ',':
			if(!input_byte())
				return false;
			break;
		case '[':
			if(!start_loop(i))
				return false;
			break;
		case ']':
			if (!end_loop(i))
				return false;
			break;
		default:
			break;
		}
		
		i++;
	}

	return true;
}


bool preprocess_line(string line, int lineNumber)
{
	int loops = 0;
	int length = line.length();

	for (int i = 0; i < length; i++)
	{
		switch (line.at(i))
		{
			case '[':
				loops++;
				break;
			case ']':
				loops--;
				break;
			default:
				break;
		}
	}

	if (loops != 0)
	{
		cout << "Error at line " << lineNumber << ": Missing \"[\" or \"]\"" << endl;
		return false;
	}

	return true;
}


bool increment_ptr()
{
	if (ptr < 30000)
	{
		ptr++;
	}
	else
		ptr = 0;

	return true;

}

bool decrement_ptr()
{
	if (ptr > 0)
		ptr--;
	else
		ptr = 30000;

	return true;
}

bool increment_val()
{
	data[ptr]++;
	return true;
}

bool decrement_val()
{
	data[ptr]--;
	return true;
}

bool output_byte()
{
	if (output == commandline)
		cout << char(data[ptr]) << endl;
	else
	{
		fout << char(data[ptr]) << endl;
	}
	return true;
}

bool input_byte()
{
	if (input == commandline)
	{
		cin.clear();
		cout << ":";
		cin >> data[ptr];
		cin.ignore();
	}
	return true;
}

bool start_loop(int &i)
{
	int j = i;
	loop_stack.push_back(j);
	return true;
}

bool end_loop(int &i)
{
	if (data[ptr] == '\0')
		loop_stack.pop_back();
	else
		i = loop_stack.back();
	return true;
}