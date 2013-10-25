// -----------------------------------------
// types.h
//
// by: Diego Rojas (github::inductionStep)
// 2011-2013
// MIT licence
// -----------------------------------------

#ifndef types_H
#define types_H

#include <vector>        // vector
#include <list>          // list
#include <queue>         // queue
#include <cstdlib>       // abs
#include <utility>       // pair
#include <cassert>       // assert

template <std::size_t TS>
struct point{
	char type;
	short int rId;
	bool visited;
	int color;//-----edit here----
	//pair<int, int> closeSeed;
	//bool border;
	unsigned char connectors[TS*TS];   //[SIZE/TS*SIZE/TS]
	
	
	point (){
		rId = -1;
		visited = false;
	}
	
	void reset(){
		rId = -1;
		for(int i = 0; i < TS*TS; i++)
			connectors[i] = -1;
	}
};

template <std::size_t TS>
struct region{
	int id;
	int visitor;//For pathfinding purposes
	std::vector<int> neightbors;
	int tile;
	
	region(){
		visitor = -1;
	}
	
	void insert(int r){
		int  n = -2;
		
		for(int i = 0; i < neightbors.size(); i++){
			if(neightbors[i] == r){
				//ALREADY NEIGHTBOR
				return;
			}
			
			if(neightbors[i] == -2){
				n = i;
			}
		}
		
		if(n == -2){
			neightbors.push_back(r);
		}else{
			neightbors[n] = r;
		}
		
		//cout << "Insertion " << (int)neightbors.size() << endl;
	}
	
	void remove(int r){
		int erase;
		
		//cout << "id: " <<  id << endl;
		if((int)neightbors.size() > TS*4 || (int)neightbors.size() < 0){
			return;
		}
		for(int i = 0; i < (int)neightbors.size(); i++){
			if(neightbors[i] == r){
				neightbors[i] = -2;//different that walls
				return;
			}
		}
		/*
		cout << "ne  ";
		for(int i = 0; i < neightbors.size(); i++){
			cout << neightbors[i] << " ";
		}
		cout << " ->, "<< endl;
		*/
	}	
	
	void disconnect(std::vector<region> &regions){
		
		//Make neightbors forget this region
		//cout << "address:  " << regions.size() << endl;
		for(int ne = 0; ne < neightbors.size(); ne++){
			//cout << "   from disconnect " << neightbors[ne] << "   " <<id << endl;
			if(neightbors[ne] != -2)
				regions[neightbors[ne]].remove(id);
		}
		
		neightbors.clear(); //Clear self list of neightbors
		id = -1;
		tile = -1;
		
		//cout << " disconnection " << (int)neightbors.size() << endl;
	}
	
	std::vector<int>& getNeightbors(){
		return neightbors;
	}
	
	int getNumberNeightbors(){
		return neightbors.size();
	}
	
};

#endif
