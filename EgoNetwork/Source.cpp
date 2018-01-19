#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


void split(const string &p_crefstrInputString, 
	string &p_crefstr1stHalf,
	string &p_crefstr2ndHalf,
	const char p_chDelimeter = ' ')
{
	size_t iSpacelocation = p_crefstrInputString.find(p_chDelimeter);

	vector<string> vtstrTokens;
	if (iSpacelocation == string::npos)
	{
		vtstrTokens.push_back(p_crefstrInputString);
		return;
	}
	p_crefstr1stHalf = p_crefstrInputString.substr(0, iSpacelocation);

	p_crefstr2ndHalf = p_crefstrInputString.substr(iSpacelocation + 1);
}

void readFileData(const string &p_crefFilePath, map<string,vector<string>> &p_mapstrvtstrFileData, const int p_ciMatrixSize)
{
	try
	{
		string line;
		ifstream myfile(p_crefFilePath);

		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				string strNum1;
				string strNum2;
				split(line, strNum1, strNum2);

				if (stoi(strNum1) >= p_ciMatrixSize)
					return;

				if (p_mapstrvtstrFileData.find(strNum1) != p_mapstrvtstrFileData.end())
				{
					vector<string> &vtstrNodes = p_mapstrvtstrFileData[strNum1];

					int iNumber2 = stoi(strNum2.c_str());
					if (iNumber2<p_ciMatrixSize)
						vtstrNodes[stoi(strNum2.c_str())] = "1";
				}
			}
			myfile.close();
		}
		else
			throw exception("Unable to open file");	
	}
	catch (exception e)
	{
		string strErrorString = "Exception occured during file reading " + string(e.what());
		throw exception(strErrorString.c_str());
	}
	catch (...)
	{
		throw exception("Unknow exception occured during file reading.");
	}
}

void writeFile(const string &p_crefstrFilepath, const string &p_crefstrText)
{
	ofstream myfile;
	myfile.open(p_crefstrFilepath);
	myfile << p_crefstrText;
	myfile.close();
}

void writeOutput(const map<string, vector<string>> &p_mapivtiFileData)
{
	string strMatrix="";
	auto itr = p_mapivtiFileData.begin();
	while (itr != p_mapivtiFileData.end())
	{
		strMatrix += itr->first;
		for (string strNode : itr->second)
		{
			strMatrix += ","+ strNode;
		}
		strMatrix += "\n";
		++itr;
	}

	writeFile("Output.txt", strMatrix);
}

void initializeMatrix(map<string, vector<string>> &p_refmapvtFileData, const int p_ciMatrixSize)
{
	int iIndex = -1;
	vector<string> vtstrNodes(p_ciMatrixSize, "0");
	iIndex = -1;
	while (++iIndex < p_ciMatrixSize)
	{
		p_refmapvtFileData.insert(pair<string, vector<string>>(to_string(iIndex), vtstrNodes));
	}
}

int validateMatrixSize(const string &p_crefMatrixSize)
{
	try
	{
		return stoi(p_crefMatrixSize.c_str());
	}
	catch (exception e)
	{
		throw exception("Matrix size should be number");
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout << "Error: Please provide 1)File path and 2) Square matrix size." << endl;
		cout << "Example: " << endl;
		cout << "\t"<<R"(C:\Users\facebook_combined.txt 30)"<<endl;
		return -1;
	}

	try
	{
		int iMatrixSize = validateMatrixSize(argv[2]);

		map<string, vector<string>> mapvtFileData;

		initializeMatrix(mapvtFileData, iMatrixSize);

		readFileData(argv[1], mapvtFileData, iMatrixSize);

		writeOutput(mapvtFileData);
	}
	catch (exception e)
	{
		cout << "Error: " << e.what() << "." << endl;
	}
	catch (...)
	{
		cout << "Error: Unknown."<< endl;
	}

	return 0;
}