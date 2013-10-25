// -----------------------------------------
// mapManager.cpp
//
// by: Diego Rojas (github::inductionStep)
// 2011-2013
// MIT licence
// -----------------------------------------


#include "mapManager.h"

template <std::size_t N, std::size_t TS>
mapManager<N, TS>::mapManager (point<TS> imap[]){
	//Table of Normals to all directions
	Ndirections[0] = 2;//5 Down
	Ndirections[1] = 3;//7 Left
	Ndirections[2] = 3;//7 Left
	Ndirections[3] = 0;//1 UP
	Ndirections[4] = 0;//1 UP
	Ndirections[5] = 1;//3 Right
	Ndirections[6] = 1;//3 Right
	Ndirections[7] = 2;//5 Down

	bool r;
	lmap = imap;
	initialize();
	//squares();
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::reset(point<TS> imap[]){
	lmap = imap;
	initialize();
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::reset(){
	initialize();
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::initialize(){
	
	regions.clear();
	
	for(int i = 0; i < N*N; i++){
		lmap[i].color = 0;
		lmap[i].reset();
	}
	
	for(int i = 0; i < N*N; i++){
		if(lmap[i].rId == -1 && lmap[i].type == 0){
			regions.push_back(divide(i, regions.size()));
		}
	}
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::line( int x, int y, int l, int d){
	for(int i = 0; i < l; i++){
		if(d == 0){
			lmap[(y+i)*N+x].type =  1;
			lmap[(y+i)*N+x].rId  = -1;
		}

		if(d == 1){
			lmap[(y)*N+x+i].type =  1;
			lmap[(y)*N+x+i].rId  = -1;
		}
	}
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::findPath(int start, int goal, int result[]){
	static std::vector<int> li;
	static std::vector<int> ne;
	int curRegion;
	int startRegion = lmap[start].rId;
	int goalRegion  = lmap[goal].rId;
	int curNeightbor = 0;
	int backTrk = 0;
	
	
	curRegion = startRegion;
	
	ne = regions[curRegion].getNeightbors();
	
	for(int n = 0; n < ne.size(); n++){
		regions[ne[n]].visitor = curRegion;
		li.push_back(regions[ne[n]].id);
		
	}
	
	for(int n = 0; n < li.size(); n++){
		curRegion = li[n];
		
		if(goalRegion == curRegion){
			backTrk = regions[goalRegion].visitor;
			for(int s = 0; backTrk != startRegion && s < 100 ;s++){
				backTrk = regions[backTrk].visitor;
				pathCache[s+1] = backTrk;
				if(backTrk != startRegion)
					pathCache[0] = s;//First element has pathLength
				
				if(s == 1000)
					break;
			}
			
			for(int q = 0; q < li.size(); q++)
				regions[li[q]].visitor = -1;
			li.clear();
			ne.clear();
			if(pathCache[0] > 0){
				for(int dex = 0; dex < pathCache[0]-1; dex++){
					if(dex == 0)
						result[0] = pathCache[0]-1;
					else
						result[dex] = connector(regions[pathCache[dex]].tile, regions[pathCache[dex+1]].tile);
				}
			}
			return;
		}
		
		for(int r = 0; r < regions[curRegion].neightbors.size(); r++){
			curNeightbor = regions[curRegion].neightbors[r];
			if(regions[curNeightbor].visitor == -1){
				li.push_back(curNeightbor);
				regions[curNeightbor].visitor = curRegion;
			}	
		}
	}
	for(int q = 0; q < li.size(); q++)
		regions[li[q]].visitor = -1;
	li.clear();
	ne.clear();
	std::cout << "Goal not found" << std::endl;
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::leaveScent(int i){
	std::vector<std::pair <int, int> > queue;
	std::pair<int, int> temp;
	int d;
	int t;
	int v;
	int oc = lmap[i].color;
	int rx, ry;
	int cx, cy;
	
	temp.first  = i;
	temp.second = 0;
	lmap[i].connectors[((i/N)%TS)*TS + (i%N)%TS] = 0;
	rx = i%N/TS;
	ry = i/N/TS;
	
	static std::pair<int, int> placements[4];
	placements[0].first = -1; placements[0].second =  0;
	placements[1].first =  0; placements[1].second =  1;
	placements[2].first =  1; placements[2].second =  0;
	placements[3].first =  0; placements[3].second = -1;
	
	queue.push_back(temp);
	
	for(int q = 0; q < queue.size(); q++){
		cx = queue[q].first%N;
		cy = queue[q].first/N;
		v  = queue[q].second;
		
		for(int a = 0; a < 4; a++){
			t = (cy+placements[a].first)*N + (cx + placements[a].second);	
			if(t >= 0 && t < N*N && lmap[t].type == 0 && lmap[t].connectors[((i/N)%TS)*TS + (i%N)%TS] == -1 && lmap[t].color == oc){
				temp.first  = t;
				temp.second = v+1;
				lmap[t].connectors[((i/N)%TS)*TS + (i%N)%TS] = (a+2)%4;
				queue.push_back(temp);
			}
		}
	}
}

template <std::size_t N, std::size_t TS>
region<TS> mapManager<N, TS>::divide(int i, int RID){
	int depth =  1;
	int gx    = -1;
	int gy    = -1;
	int rx    = (i%N)/TS;
	int ry    = (i/N)/TS;
	//int RID   = lmap[i].rId;
	int x     = i%N;
	int y     = i/N;
	bool enclosed = true;
	int color = random(999999);
	bool found;
	region<TS> rs;
	std::vector<int> queue;
	std::vector<int> result;
	
	rs.id = RID;
	rs.tile = i;
	queue.push_back(i);
	
	for(int q = 0; q < queue.size(); q++){
		x = queue[q]%N;
		y = queue[q]/N;
		result.push_back(y*N+x);
		gx=-depth;
		gy=-depth;
		for(int j = 0; j < (1+2*depth)*(1+2*depth); j++){
			if(0 <= (y+gy)*N+(x+gx) && (y+gy)*N+(x+gx) < N*N && (x+gx) >= 0 && (y+gy) >= 0 && (x+gx) < N && (y+gy) < N && abs(gx)+abs(gy) == 1){
				if(lmap[(y+gy)*N+(x+gx)].type == 0){
					if((x+gx)/TS == rx && (y+gy)/TS == ry){
						enclosed = false;//Its not surrounded by unwalkable
						if(lmap[(y+gy)*N+(x+gx)].rId != RID){

							lmap[(y+gy)*N+(x+gx)].rId = RID;
							lmap[(y+gy)*N+(x+gx)].color = color;
							queue.push_back((y+gy)*N+(x+gx));
						}
					}else{
						found = false;
						for(int w = 0; w < rs.neightbors.size(); w++){
							if(rs.neightbors[w] == lmap[(y+gy)*N+(x+gx)].rId){
								found = true;
								break;
							}
						}
						if(!found && lmap[(y+gy)*N+(x+gx)].rId != -1){
							rs.neightbors.push_back(lmap[(y+gy)*N+(x+gx)].rId);
						}
					}
				}
			}
			if(gx == depth){
				gx = -depth;
				gy++;
			}else{
				gx++;
			}
		}
	}
	if(enclosed){
		lmap[i].rId = RID;
	}
	for(int a = 0; a < result.size(); a++){
		////cout << result[a] << " ";
		leaveScent(result[a]);
	}			
	for(int e = 0; e < rs.neightbors.size(); e++){
		regions[rs.neightbors[e]].insert(rs.id);
	}
	return rs;
}

template <std::size_t N, std::size_t TS>
bool mapManager<N, TS>::closedRegion(int start, int goal, int d){
	int cx;
	int cy;
	int t;
	int t2;
	int t3;
	int to;
	int stepCount = 0;
	int Limit = (TS)*(TS);
	int starHits = 0;
	int erase = 0;
	int dcount = 0;
				
	cx = start%N;
	cy = start/N;
			
	static std::pair<int, int> placements[4];
	placements[0].first = -1; placements[0].second =  0;//UP
	placements[1].first =  0; placements[1].second =  1;//RIGHT
	placements[2].first =  1; placements[2].second =  0;//DOWN
	placements[3].first =  0; placements[3].second = -1;//LEFT
	
	static std::pair<int, int> normals[4];
	normals[0].first = -1;    normals[0].second = -1;//UP-LEFT
	normals[1].first = -1;    normals[1].second =  1;//UP-RIGHT
	normals[2].first =  1;    normals[2].second =  1;//DOWN-RIGHT
	normals[3].first =  1;    normals[3].second = -1;//DOWN-LEFT
	
	to = (cy+placements[d].first)*(int)N + (cx + placements[d].second);
	d = (d+1)%4;
	while(stepCount < Limit){
		t =  (cy+placements[d].first)*(int)N + (cx + placements[d].second);
		t2 = (cy+placements[(4+(d-1))%4].first)*N + (cx + placements[(4+(d-1))%4].second);
		t3 = (cy+normals[(4+(d-1))%4].first)*N + (cx + normals[(4+(d-1))%4].second);

		if(cy*N+cx == goal){
			return true;
		}
		if(cy*N+cx == start && ((t2 == to && stepCount > 0) || starHits == 2)){
			return false;
		}
		if(cy*N+cx == start){
			starHits++;
		}
		if(lmap[t2].type == 0 && (lmap[t2].rId == lmap[start].rId)){
			if(t3 < 0)
				d = (4+(d-1))%4;
			else if(lmap[t3].type == 1)
				d = (4+(d-1))%4;
		}else{
			dcount = 0;
			while(lmap[t].type == 1 || (lmap[t].rId != lmap[start].rId)){
				if(dcount == 4)
					return false;
				
				d = (d+1)%4;
				t =  (cy+placements[d].first)*(int)N + (cx + placements[d].second);
				dcount++;
			}
		}
		t =  (cy+placements[d].first)*(int)N + (cx + placements[d].second);	
		if(lmap[t].type == 0){
			cy = cy + placements[d].first;
			cx = cx + placements[d].second;
			
		}
		stepCount++;	
	}
	
	return false;
}

template <std::size_t N, std::size_t TS>
std::vector<std::pair <int, int> > mapManager<N, TS>::regionTest(int o){
	int x  = o%N;
	int y  = o/N;
	int t  = 0; 
	int s  = -1;
	bool b;
	std::pair<int, int> cr;
	std::pair<int, int> fr;
	std::pair<int, int> tm;
	std::vector<std::pair < int, int> > rs;
	bool w;
	int ps[8] = {-1,-1,-1,-1,-1,-1,-1,-1}; 
	int erase;
	
	fr = toRegion(o);
	
	static std::pair<int, int> placements[8];
	placements[0].first = -1; placements[0].second = -1;
	placements[1].first = -1; placements[1].second =  0;
	placements[2].first = -1; placements[2].second =  1;
	placements[3].first =  0; placements[3].second =  1;
	placements[4].first =  1; placements[4].second =  1;
	placements[5].first =  1; placements[5].second =  0;
	placements[6].first =  1; placements[6].second = -1;
	placements[7].first =  0; placements[7].second = -1;
	placements[8].first = -1; placements[8].second = -1;//EXTRA TO MAKE IT WORK
	
	for(int i = 0; i < 9; i++){
		t = (y+placements[i%8].first)*N + x+placements[i%8].second;
		if(t >= 0 && t < N*N && (y+placements[i%8].first) < N && x+placements[i%8].second < N){
			
			if(lmap[t].type == 0){
				cr = toRegion(t);
				if(ps[(8+(i-1))%8] == -1 || lmap[t].rId != lmap[abs(ps[(8+(i-1))%8])].rId){
					ps[i%8] = t;
					b = i > 0;
					if(rs.size() > 0){
						if(t == rs[0].first){
							b = false;
						}
					}
					if(lmap[o].rId == lmap[t].rId && b){
						tm.first  = t;
						tm.second = i%8;
						rs.push_back(tm);
					}
				}else{
					ps[i%8] = -t;
				}
			}
		} 
	}
	return rs;
}

template <std::size_t N, std::size_t TS>
std::pair<int, int> mapManager<N, TS>::toRegion(int i){
	std::pair<int, int> r;
	r.first  = -1;
	r.second = -1;
	
	if(i >= 0 && i < N*N){
		r.first  = (i/N)/TS;
		r.second = (i%N)/TS;
	}
	return r;
}

template <std::size_t N, std::size_t TS>
bool mapManager<N, TS>::borderTest(int i, int l){
	int depth = 1;
	int gx    = -1;
	int gy    = -1;
	int x = i%N;
	int y = i/N;
	static int ox;
	static int oy;
	static bool ch;
	static bool r;
	
	if(l == 0){
		if(!isBorder(i)){
			return false;
		}
		ox = i%N;
		oy = i/N;
		r  = false;
	}
	
	static std::vector<int> nodes;
	
	if(lmap[i].type == 1){
		lmap[i].type = -1;
		nodes.push_back(i);
		gx=-depth;
		gy=-depth;
		for(int j = 0; j < (1+2*depth)*(1+2*depth); j++){
			if(0 <= (y+gy)*N+(x+gx) && (y+gy)*N+(x+gx) < N*N && (x+gx) >= 0 && (y+gy) >= 0 &&  lmap[(y+gy)*N+(x+gx)].type == 1){
				if(ox/TS == (x+gx)/TS && oy/TS == (y+gy)/TS){
					if(isBorder((y+gy)*N+(x+gx))){
						r = true;
					}else{
						borderTest((y+gy)*N+(x+gx), l+1);
					}
				}
			}
			if(gx == depth){
				gx = -depth;
				gy++;
			}else{
				gx++;
			}
		}
	}
	if(l == 0){
		for(int i = 0; i < nodes.size(); i++){
			lmap[nodes[i]].type = 1;
			if(r)
				lmap[nodes[i]].color = 777777; 
			else
				lmap[nodes[i]].color = 000000; 
		}
		nodes.erase(nodes.begin(),nodes.end());
	}
	
	return r;
}

template <std::size_t N, std::size_t TS>
bool mapManager<N, TS>::isBorder(int i){
	int x, y, bx, by;
	
	x  = i%N;
	y  = i/N;
	bx = x/TS;
	by = y/TS;
	
	if(x == bx*TS || x == (bx*TS + TS-1) || y == by*TS || y == (by*TS + TS-1)){
		return true;
	}
	return false;
}

template <std::size_t N, std::size_t TS>
bool mapManager<N, TS>::isBorder(int i, int &dir){
	int x, y, bx, by;
	
	x  = i%N;
	y  = i/N;
	bx = x/TS;
	by = y/TS;
	
	if((y % TS == 0 || y % TS == TS -1) && (x % TS == 0 || x % TS == TS -1)){
		dir = 2;
		return true;
	}
	if(x == bx*TS || x == (bx*TS + TS-1)){
		dir = 0;
		return true;
	}
	if(y == by*TS || y == (by*TS + TS-1)){
		dir = 1;
		return true;
	}
	return false;
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::activeBorder(int i, int result[]){
	int x, y, curx, cury, nx, ny, strt, t, tx;
	int dir;
	int open = 0;
	int RID, NRID;
	
	if(isBorder(i, dir)){
		x  = i%N;
		y  = i/N;
		
		if(x == 0 || x == N-1 || y == 0 || y == N-1){
			return;
		}
		NRID = lmap[y*N+x].rId;
		if(dir == 0 || dir == 2){
			RID  = lmap[y*N+((-1 + (x % 2)*2) + x)].rId;
			strt = y - y % TS;
			for(int j = 0; j < TS; j++){
				cury = strt + j;
				t  = cury*N+x;
				tx = cury*N+((-1 + (x % 2)*2) + x);
				if(lmap[t].type == 0 && lmap[tx].type == 0 && lmap[tx].rId == RID && lmap[t].rId == NRID){
					open++;
					if(open == 2)
						break;
				}
			}
			if(open){
				result[0] = open;
				result[1] = RID;
				open = 0;
			}
		}
		if(dir == 1 || dir == 2){
			RID = lmap[((-1 + (y % 2)*2) + y)*N+x].rId;
			strt = x - x % TS;
			for(int j = 0; j < TS; j++){
				curx = strt + j;
				t  = y*N+curx;
				tx = ((-1 + (y % 2)*2) + y)*N+curx;
				if(lmap[t].type == 0 && lmap[tx].type == 0 && lmap[tx].rId == RID && lmap[t].rId == NRID){
					open++;
					if(open == 2)
						break;
				}
			}
			if(open){
				result[2] = open;
				result[3] = RID;
			}
		}
	}
}

template <std::size_t N, std::size_t TS>
int mapManager<N, TS>::getNewRegionID(int dontUse){
	region<TS> dummy;
	for(int i = 0; i < regions.size(); i++){
		if(regions[i].id == -1 && i != dontUse){
			return i;
		}
	}
	return -1;
}

template <std::size_t N, std::size_t TS>
int mapManager<N, TS>::getNewRegionID(std::vector< std::pair <int, int> > &dontUse){
	int pos = -1;
	bool pas;
	
	for(int i = 0; i < regions.size(); i++){
		pas = true;
		if(regions[i].id == -1){
			for(int q = 0; q < dontUse.size(); q++){
				if(i == dontUse[q].first){
					pas = false;
				}
			}
			if(pas){
				return i;
			}
		}
	}
	return -1;
}

template <std::size_t N, std::size_t TS>
int mapManager<N, TS>::getNewRegionID(std::list<int> &dontUse){
	bool pas;
	
	for(int i = 0; i < regions.size(); i++){
		pas = true;
		if(regions[i].id == -1){
			for (std::list<int>::iterator it=dontUse.begin(); it!=dontUse.end(); ++it){
				if(i == *it){
					pas = false;
				}
			}
			if(pas){
				return i;
			}
		}
	}
	return -1;
}

template <std::size_t N, std::size_t TS>
bool mapManager<N, TS>::lonelyTile(int x, int y){
	int RID = lmap[y*N+x].rId;
	int t;
	
	static std::pair<int, int> placements[4];
	placements[0].first = -1; placements[0].second =  0;//UP
	placements[1].first =  0; placements[1].second =  1;//RIGHT
	placements[2].first =  1; placements[2].second =  0;//DOWN
	placements[3].first =  0; placements[3].second = -1;//LEFT
	
	for(int i = 0; i < 4; i++){
		t = (y+placements[i].first)*N + x+placements[i].second;
		if(lmap[t].rId == RID)
			return false;
	}
	return true;
}

template <std::size_t N, std::size_t TS>
int mapManager<N, TS>::connector(int r1, int r2){
	int r1x, r2x, r1y, r2y, id1, id2, t, tn, cur, strt, temp, swap;
	
	lmap[r1].color = 0;
	lmap[r2].color = 0;
	
	swap = 0;
	if(r1 < r2){
		temp = r1;
		r1 = r2;
		r2 = temp;
		swap = 1;
	}
	
	r1x = r1%N/TS*TS;
	r1y = r1/N/TS*TS;
	r2x = r2%N/TS*TS;
	r2y = r2/N/TS*TS;
	
	id1 = lmap[r1].rId;
	id2 = lmap[r2].rId;
	
	if(fabs(r1y - r2y) == 0){
		strt = r1y - r1y % TS;
		for(int j = 0; j < TS; j++){
			cur = strt + j;
			t  = cur*N+r1x;
			tn = cur*N+((-1 + (r1x % 2)*2) + r1x);
			if(lmap[tn].rId == id2 && lmap[t].rId == id1){
				if(swap){
					return tn;
				}
				return t;
			}
		}
	}else if(fabs(r1x - r2x) == 0){
		strt = r1x - r1x % TS;
		for(int j = 0; j < TS; j++){
			cur = strt + j;
			t  = r1y*N+cur;
			tn = ((-1 + (r1y % 2)*2) + r1y)*N+cur;
			if(lmap[tn].rId == id2 && lmap[t].rId == id1){
				if(swap){
					return tn;
				}
				return t;
			}
		}
	}
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::insert(int x, int y){
	lmap[y*N+x].type = 1;
	lmap[y*N+x].color    = 000000;
	int d;
	int r;
	region<TS> ne;
	int pr[4] = {0};
	
	std::vector< std::pair <int, int> > ps;
	ps = regionTest(y*N+x);

	if(ps.size() == 0){
		if(lonelyTile(x, y)){
			regions[lmap[y*N+x].rId].id = -1;
		}		
		lmap[y*N+x].rId  = -1;
		return;
	}
	for(int i = 0; i < ps.size()-1; i++){
		for(int j = i+1; j < ps.size(); j++){
			d = Ndirections[ps[i].second];
			if(ps[j].first != -1 && ps[i].first != -1){
				if(closedRegion(ps[i].first, ps[j].first, d)){
					ps[j].first = -1;
				
				}
			}
		}
	}
	for(int i = 0; i < ps.size()-1; i++){
		for(int j = i+1; j < ps.size(); j++){
			d = Ndirections[ps[i].second];
			if(ps[j].first != -1 && ps[i].first != -1){
				if(!closedRegion(ps[i].first, ps[j].first, d)){
					regions[lmap[y*N+x].rId].disconnect(regions);
					if(pr[i] == 0){	
						r = getNewRegionID(lmap[y*N+x].rId);
						if(r == -1){
							regions.push_back(divide(ps[i].first, regions.size()));
						}else{
							regions[r] = divide(ps[i].first, r);
						}
						pr[i] = 1;
					}
					if(pr[j] == 0){
						r = getNewRegionID(lmap[y*N+x].rId);
						if(r == -1){
							regions.push_back(divide(ps[j].first, regions.size()));
						}else{
							regions[r] = divide(ps[j].first, r);
						}
						pr[j] = 1;
					}
				}
			}
		}
	}
	if(ps.size() == 1){
		int result[4] = {0};
		lmap[y*N+x].type = 0; 
		activeBorder(y*N+x, result);
		
		if(result[0] == 1){
			regions[lmap[y*N+x].rId].remove(result[1]);
			regions[result[1]].remove(lmap[y*N+x].rId);
		}
		if(result[2] == 1){
			regions[lmap[y*N+x].rId].remove(result[3]);
			regions[result[3]].remove(lmap[y*N+x].rId);
		}
		lmap[y*N+x].type = 1;
		scentRegion(ps[0].first);
	}
	lmap[y*N+x].rId  = -1;	
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::scentRegion(int i){
	int x = i%N;
	int y = i/N;
	int t;
	int RID = lmap[i].rId;
	std::vector<int> nodes;
	
	static std::pair<int, int> placements[4];
	placements[0].first = -1; placements[0].second =  0;//UP
	placements[1].first =  0; placements[1].second =  1;//RIGHT
	placements[2].first =  1; placements[2].second =  0;//DOWN
	placements[3].first =  0; placements[3].second = -1;//LEFT
	
	nodes.push_back(i);
	
	for(int q = 0; q < nodes.size(); q++){
		x = nodes[q]%N;
		y = nodes[q]/N;
		lmap[nodes[q]].visited = true;
		leaveScent(nodes[q]);
		for(int d = 0; d < 4; d++){
			t = (y+placements[d].first)*(int)N + (x + placements[d].second);
			if(t >= 0 && t < N*N && !lmap[t].visited && lmap[t].rId == RID){
				nodes.push_back(t);
			}
		}
	}
	for(int w = 0; w < nodes.size(); w++){
		lmap[nodes[w]].visited = false;
	}
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::showScent(int i){			
	int cx, cy, pos, oc, sx, sy;
	for(int u = 0; u < regions[lmap[i].rId].neightbors.size(); u++){
		for(int b = 0; b < N*N; b++){
			if(lmap[b].rId == regions[lmap[i].rId].neightbors[u]){
				lmap[b].color = 757489;
			}
		}
	}				
	
	cx = (i%N)%TS;
	cy = (i/N)%TS;
	oc = lmap[i].color;
	sx = ((i%N)/TS)*TS;
	sy = ((i/N)/TS)*TS;
	pos = cy*TS + cx;
	for(int y = 0; y < TS; y++){
		for(int x = 0; x < TS; x++){
			if(lmap[(sy+y)*N+(sx+x)].color == oc){
				lmap[(sy+y)*N+(sx+x)].color = lmap[(sy+y)*N+(sx+x)].connectors[pos]*600;
			}
		}
	}
}

template <std::size_t N, std::size_t TS>
void mapManager<N, TS>::remove(int x, int y){
	int location = y*N+x;
	
	if(lmap[location].type == 0)
		return;
	
	int rx = x/TS;
	int ry = y/TS;
	int c = -1;
	int t;
	int r;
	int acolor = 0;
	int RID;
	int result[4] = {0.0};
	std::vector< std::pair <int, int> > ps;
	std::list<int> mylist;

	RID = lmap[location].rId;
	static std::pair<int, int> placements[8];
	placements[0].first = -1; placements[0].second =  0;
	placements[1].first =  0; placements[1].second =  1;
	placements[2].first =  1; placements[2].second =  0;
	placements[3].first =  0; placements[3].second = -1;

	lmap[location].type = 0;
	lmap[location].color    = 999999; 
	for(int i = 0; i < 4;i++){
		t = (y+placements[i].first)*N + x + placements[i].second;
		if(0 <= t && t < N*N && (x+placements[i].second)/TS == rx && (y+placements[i].first)/TS == ry && lmap[t].type == 0){
			
			
			mylist.push_back(lmap[t].rId);
			acolor = lmap[t].color;
		}
	}
	mylist.unique();
	if(mylist.size() != 1){
		for (std::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it){
			regions[*it].disconnect(regions);
		}
		
		r  = getNewRegionID(mylist);
		if(r == -1){
			regions.push_back(divide(location, regions.size()));
		}else{
			regions[r] = divide(location, r);
		}
	}else{
		lmap[location].rId = mylist.front();
		lmap[location].color = acolor;
	}
	activeBorder(location, result);
	if(result[0] == 1){
		regions[lmap[location].rId].insert(result[1]);
		regions[result[1]].insert(lmap[location].rId);
	}
	if(result[2] == 1){
		regions[lmap[location].rId].insert(result[3]);
		regions[result[3]].insert(lmap[location].rId);
	}
	return;	
}

template <std::size_t N, std::size_t TS>
mapManager<N, TS>::~mapManager () {
    //delete [] map;
}

template <std::size_t N, std::size_t TS>
int mapManager<N, TS>::random(int range){
	return rand() % range;
}

