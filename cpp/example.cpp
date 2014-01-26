// -----------------------------------------
// example.cpp
//
// by: Diego Rojas (github::inductionStep)
// 2011-2013
// MIT licence
// -----------------------------------------


/*
To run program
	% g++ example.cpp -o example 
	% ./example

*/


#include <iostream>      // cout, endl
#include "types.h"       // point, region 
#include "mapManager.h"  // manager
#include "mapManager.cpp"  // manager

using namespace std;

const int SIZE = 300;
const int TS   = 12;


//------------------------//
/*----HELPER FUNCTIONS----*/
//------------------------//
int _2d_to_1d(int x, int y, int width){
	return y*width + x;
}

void printConnectors(int arr[]){
	for(int index = 1; index < arr[0]; index++){
		cout << arr[index] << " ";
	}
	cout << endl;
}

string printMap(point<TS> map[]) {
	string rs = "";
	cout << "<table>";
	for(int y = 0; y < SIZE; y++) {
		cout << "<tr>" << endl
		for(int x = 0; x < SIZE; x++) {
			rs += "<td style=\"background-color:";
			rs += 
		}
		cout << "</tr>" << endl;
	}
	cout << "</table>";
}


int main (int argc, char** argv) {
	vector< point<TS> > map;
	map.resize(SIZE*SIZE);
	mapManager<SIZE, TS> manager(map.data());
	int path[100]; // lives on the stack
					// First element has pathLength !IMPORTANT
	int startX, endX, startY, endY, cont;
	
	
	
	manager.insert(10, 10);
	manager.remove(10, 10);
	manager.insert(11, 11);
	manager.insert(12, 12);
	manager.insert(13, 13);
	manager.insert(14, 14);
	manager.insert(15, 15);
	manager.remove(12, 12);
	
	while(true){
		cout << "start X: ";
		cin  >> startX;
		cout << "\nstartY: ";
		cin  >> startY;
		cout << "end X: ";
		cin  >> endX;
		cout << "\nend Y: ";
		cin  >> endY;	
		
		manager.findPath(_2d_to_1d(startX, startY, SIZE), _2d_to_1d(endX, endY, SIZE), path);
		printConnectors(path);
		
		cout << "continue?: (y/n)" << endl;
		cin  >> cont;
		if(cont != 'y')
			break;
	}
	

	
	
    return 0;
}



