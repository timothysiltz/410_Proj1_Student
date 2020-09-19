/***
 * KP Utils
 *
 * Tim Siltz
 * 9/19/20
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

std::vector<process_stats> stats;

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	if (*myString == UNINITIALIZED){
		return 0;
	}

	return atoi(myString);
}

//clears vector (or whatever datastructure you choose) holding process_stats structs
//attempt to open file 'filename' to read, parse its rows
//into process_stats structs and add these structs to the vector holding these structs
//ignore malformed rows
//if ignoreFirstRow ==true then discard the first row
//returns SUCCESS if all goes well or COULD_NOT_OPEN_FILE
int loadData(const char* filename, bool ignoreFirstRow) {
	stats.clear();

	ifstream myFile;
	myFile.open(filename);

	if (!myFile.is_open()){
		return COULD_NOT_OPEN_FILE;
	}

	stringstream ss;
	string line;
	string token;

	if (ignoreFirstRow == true){
		getline(myFile, line);
	}

	while (!myFile.eof()){
		getline(myFile, line);
		ss.str(line);

		string::iterator itr;
		process_stats temp;
		int comma = 0;
		bool corrupted = false;

		for (itr = line.begin(); itr != line.end(); itr++){
			if (*itr == CHAR_TO_SEARCH_FOR){
				comma++;
			}
		}

		getline(ss, token, CHAR_TO_SEARCH_FOR);
		temp.process_number = atoi(token.c_str());
		if (token.empty() && atoi(token.c_str()) == 0){
			corrupted = true;
		}

		getline(ss, token, CHAR_TO_SEARCH_FOR);
		temp.start_time = atoi(token.c_str());
		if (token.empty() && atoi(token.c_str()) == 0){
			corrupted = true;
		}

		getline(ss, token, CHAR_TO_SEARCH_FOR);
		temp.cpu_time = atoi(token.c_str());
		if (token.empty() && atoi(token.c_str()) == 0){
			corrupted = true;
		}

		getline(ss, token, CHAR_TO_SEARCH_FOR);
		temp.io_time = atoi(token.c_str());
		if (token.empty() && atoi(token.c_str()) == 0){
			corrupted = true;
		}

		if (comma != 3){
			corrupted = true;
		}

		if(corrupted == false){
			stats.push_back(temp);
		}

		ss.clear();
	}

	if(myFile.is_open()){
		myFile.close();
	}

	return SUCCESS;
}

bool compareProcess(const process_stats& s1, const process_stats& s2){
	return (s1.process_number < s2.process_number);
}

bool compareStart(const process_stats& s1, process_stats& s2){
	return (s1.start_time < s2.start_time);
}

bool compareCPU(const process_stats& s1, const process_stats& s2){
	return (s1.cpu_time < s2.cpu_time);
}

bool compareIO(const process_stats& s1, const process_stats& s2){
	return (s1.io_time < s2.io_time);
}

//will sort according to user preference (low to high)
void sortData(SORT_ORDER mySortOrder) {
	switch (mySortOrder){
		case CPU_TIME:
			sort(stats.begin(), stats.end(), compareCPU);
			break;
		case PROCESS_NUMBER:
			sort(stats.begin(), stats.end(), compareProcess);
			break;
		case START_TIME:
			sort(stats.begin(), stats.end(), compareStart);
			break;
		case IO_TIME:
			sort(stats.begin(), stats.end(), compareIO);
			break;
	}
}

//return the first struct in the vector
//then deletes it from the vector
process_stats getNext() {
	process_stats myFirst;
	myFirst = stats.front();
	stats.erase(stats.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return stats.size();
}
