// -----------------------------------------
// mapManager.h
//
// by: Diego Rojas (github::inductionStep)
// 2011-2013
// MIT licence
// -----------------------------------------


#ifndef mapManager_H
#define mapManager_H

#include <iostream>      // cout, endl
#include <vector>        // vector
#include <list>          // list
#include <math.h>        // fabs
#include <cstdlib>       // abs
#include <utility>       // pair
#include <cassert>       // assert
#include "types.h"       // point, region 

template <std::size_t N, std::size_t TS>
class mapManager {
	
	point<TS> *lmap;
	std::vector<region<TS> > regions;
	int Ndirections[8];
	int pathCache[100];
		
	public:
		
		// ------------
		// constructor
		// ------------
		/**
		* Constructor
		* @param imap an array of type point
		*/
		template <std::size_t N, std::size_t TS>
        mapManager (point<TS> imap[]);
		
		
		// ------------
		// destructor
		// ------------
		/**
		* destructor
		*/
		~mapManager ();
		
		
		// ------------
		// findPath
		// ------------
		/**
		* Find path between two points(start, goal)
		*  points are translation of a 2d
		*  array into a single dimention
		* @param start an int
		* @param goal an int
		* @param result an array passed by reference
		*/
		void findPath(int start, int goal, int result[]);
		
		
		// ------------
		// reset
		// ------------
		/**
		* reset 
		* @param imap an array of type point
		*/
		void reset(point<TS> imap[]);
	
	
		// ------------
		// reset
		// ------------
		/**
		* reset
		*/
		void reset();
		
		
		// ------------
		// initialize
		// ------------
		/**
		* initialize data structure
		*/
		void initialize();
		
		
		// ------------
		// random
		// ------------
		/**
		* @param  range an int
		* @return random integer between range
		*/
		int random(int range);
		
		
		// ------------
		// insert
		// ------------
		/**
		* Insert a wall into position passed as (x,y) 
		* @param int x 
		* @param int y
		*/
		void insert(int x, int y);
		
		
		// ------------
		// remove
		// ------------
		/**
		* Recome a wall
		* @param x an int
		* @param x an int
		*/
		void remove(int x, int y);

	private:		
		// ------------
		// line
		// ------------
		/**
		* Make a line of tiles unwalkable
		* @param x an int
		* @param y an int
		* @param l an int
		* @param d an int
		*/
		void line(int x, int y, int l, int d);
				
		
		// ------------
		// leaveScent
		// ------------
		/**
		* leaveScent
		* Leave the scent of a tile onto the other tiles
		*  sharing the same region
		* @param i an int representing the 1d translation of the position of the point
		*/
		void leaveScent(int i);


		// ------------
		// divide
		// ------------
		/**
		* divide a region
		* @param i an int
		* @param RID an int representing regionID
		* @return region type instance of new region created by division
		*/
		region<TS> divide(int i, int RID);
		
		
		// ------------
		// closedRegion
		// ------------
		/**
		* Determines if after an insertion/deletion a region is open 
		*  or closed
		* @param start an int
		* @param goal an int 
		* @param d an int determining start direction
		* @return true if region closed, false otherwise
		*/
		bool closedRegion(int start, int goal, int d);
		
		
	    // ------------
	    // regionTest
	    // ------------
	    /**
	    * Get the region of the tile passed as parameter
	    * @param int o 
	    * @return vector<pair <int, int> >
	    */
		std::vector<std::pair <int, int> > regionTest(int o);
		
		
		// ------------
		// toRegion
		// ------------
		/**
		* Map a point into its region
		* @param i an int
		* @return pair<int, int> first=y, second 
		*/
		std::pair<int, int> toRegion(int i);
		
		
		// ------------
		// borderTest
		// ------------
		/**
		* Test whether two regions are unreachable from each other
		* @param i an int from region A
		* @param j an int from region B
		* @return true if connected, false if otherwise
		*/
		bool borderTest(int i, int l);
		
		
		// ------------
		// isBorder
		// ------------
		/**
		* Tells whether a tile is a border between two regions
		* @param i and int
		* @return true if border, false otherwise
		*/
		bool isBorder(int i);
		
		
		// ------------
		// isBorder
		// ------------
		/**
		* Tells whether a tile is a border between two regions
		*  stores the direction normal in &dir
		* @param r   an int 
		* @param dir an int passed by reference
		* @return true if border, false otherwise
		*/
		bool isBorder(int i, int &dir);
		
		
		// ------------
		// activeBorder
		// ------------
		/**
		* reads two ints into i and j
		* @param r a  std::istream
		* @param i an int by reference
		* @param j an int by reference
		* @return true if that succeeds, false otherwise
		*/
		void activeBorder(int i, int result[]);
		
		
		// ------------
		// getNewRegionID
		// ------------
		/**
		* Get new id for newly created region
		* @param int dontUse 
		* @return the id as int
		*/
		int getNewRegionID(int dontUse);
		
		
		// --------------
		// getNewRegionID
		// --------------
		/**
		* Get new id for newly created region
		* @param dontUse a vector< pair <int, int> >  passed by reference
		* @return the id as an int
		*/
		int getNewRegionID(std::vector< std::pair <int, int> > &dontUse);
		
		
		// --------------
		// getNewRegionID
		// --------------
		/**
		* Get new id for newly created region
		* @param dontUse and list<int> passed by reference
		* @return id as int
		*/
		int getNewRegionID(std::list<int> &dontUse);
		
		
		// ------------
		// lonelyTile
		// ------------
		/**
		* Check if a tile is enclosed
		* @param x an int
		* @param y an int
		* @return true if tile enclosed, false otherwise
		*/
		bool lonelyTile(int x, int y);
		
		
		// ------------
		// connector
		// ------------
		/**
		* Takes two integers, each a point in a region
		* Returns a point such that it is open and common to both regions
		* This point can be used to cross between the regions
		* @param r1 an int representing the id of a region
		* @param r2 an int representing the id of a region
		* @return int representing the 2d translatetion of the connecting tile
		*/
		int connector(int r1, int r2);
		

		// ------------
		// scentRegion
		// ------------
		/**
		* Leave scent of a tile inside its enclosing region 
		* @param i an int
		*/
		void scentRegion(int i);
		
		
		// ------------
		// showScent
		// ------------
		/**
		* showScent, show the scent for a tile inside a region
		* @param i an int representing the 1d translation of the position of the point
		*/
		void showScent(int i);
		
};

#endif
