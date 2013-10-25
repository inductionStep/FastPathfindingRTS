// -----------------------------------------
// example.cpp
//
// by: Diego Rojas (github::inductionStep)
// 2011-2013
// MIT licence
// -----------------------------------------


#include <iostream>      // cout, endl
#include "types.h"       // point, region 
#include "mapManager.h"  // manager

using namespace std;

const int SIZE = 200;
const int TS   = 10;


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


int main (int argc, char** argv) {
	point<TS> map[SIZE*SIZE];
	mapManager<SIZE, TS> manager(map);
	int path[100]; // lives on the stack
					// First element has pathLength !IMPORTANT
	
	manager.insert(10, 10);
	manager.remove(10, 10);
	manager.insert(11, 11);
	manager.insert(12, 12);
	manager.insert(13, 13);
	manager.insert(14, 14);
	manager.insert(15, 15);
	manager.remove(12, 12);
	
	manager.findPath(_2d_to_1d(10, 10, SIZE), _2d_to_1d(150, 10, SIZE), path);
	printConnectors(path);
	
	
    return 0;
}



