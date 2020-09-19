/***
 * KP Utils
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
	if (*myString == NULL){
		return 0;
	}

	return atoi(myString);
}

//clears vector (or whatever datastructure you choose) holding process_stats structs
//attempt to open file 'filename' to read, parse its rows
//into process_stats structs and add these structs to the vector holding these structs
//NOTE: be sure to ignore malformed rows (see 'Project 1 Description')
//if ignoreFirstRow ==true then discard the first row
//returns SUCCESS if all goes well or COULD_NOT_OPEN_FILE
int loadData(const char* filename, bool ignoreFirstRow) {
	stats.clear();

	ifstream myFile;
	stringstream ss;
	myFile.open(filename);

	if (!myFile.is_open()){
		return COULD_NOT_OPEN_FILE;
	}

	string line;
	string numString;
	process_stats p;

	if (ignoreFirstRow == true){
		getline(myFile, line);
	}

	while (!myFile.eof()){
		getline(myFile, line);
		stringstream ss(line);

		getline(ss, numString, CHAR_TO_SEARCH_FOR);
		p.process_number = stoi(numString);
		getline(ss, numString, CHAR_TO_SEARCH_FOR);
		p.start_time = stoi(numString);
		getline(ss, numString, CHAR_TO_SEARCH_FOR);
		p.cpu_time = stoi(numString);
		//getline(ss, numString, CHAR_TO_SEARCH_FOR);
		getline(ss, numString);
		p.io_time = stoi(numString);

		stats.push_back(p);

		return SUCCESS;
	}

	if(myFile.is_open()){
		myFile.close();
	}

	//return COULD_NOT_OPEN_FILE;
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
