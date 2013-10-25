function Node (x, y, type) {
	this.closed = false;
	this.parent = null;
	this.type = type;
	this.visited = false;
	this.x = x;
	this.y = y;
}

function Grid (grid) {
	var nodes = [];

	for (var y = 0, yl = grid.length; y < yl; y++) {
		nodes.push([]);
		for (var x = 0, xl = grid[y].length; x < xl; x++) {
			if (grid[y][x].blocked) {
				nodes[y].push(new Node(x, y, 1));
			}
			else {
				nodes[y].push(new Node(x, y, 0));
			}
		}
	}

	this.grid = grid;
	this.nodes = nodes;
}

function Neighbors (grid, node, diagonals) {
	var ret = [];
	var x = node.x;
	var y = node.y;

	if (grid[y-1] && grid[y-1][x]) {
		ret.push(grid[y-1][x]);
	}
	if (grid[y+1] && grid[y+1][x]) {
		ret.push(grid[y+1][x]);
	}
	if (grid[y] && grid[y][x-1]) {
		ret.push(grid[y][x-1]);
	}
	if (grid[y] && grid[y][x+1]) {
		ret.push(grid[y][x+1]);
	}
	if (diagonals) {
		if (grid[y-1] && grid[y-1][x-1]) {
			ret.push(grid[y-1][x-1]);
		}
		if (grid[y+1] && grid[y+1][x-1]) {
			ret.push(grid[y+1][x-1]);
		}
		if (grid[y-1] && grid[y-1][x+1]) {
			ret.push(grid[y-1][x+1]);
		}
		if (grid[y+1] && grid[y+1][x+1]) {
			ret.push(grid[y+1][x+1]);
		}
	}

	return ret;
}

function Map (grid, start, end) {
	var open = [];

	open.push(start);

	while (open.length > 0) {
		var current = open.splice(0, 1)[0];

		if (current === end) {
			var ret = [];
			while (current.parent) {
				ret.push(current);
				current = current.parent;
			}
			return ret.reverse();
		}

		current.closed = true;

		var neighbors = Neighbors(grid, current);

		for (var i = 0, il = neighbors.length; i < il; i++) {
			var n = neighbors[i];
			if (n.closed || n.type === 1) {
				continue;
			}
			if (!n.visited) {
				n.parent = current;
				n.visited = true;
				open.push(n);
			}
		}
	}

	return [];
}


// tests

var MAP_HEIGHT = 100;
var MAP_WIDTH = 100;
var BLOCK_SIZE = 2;

var grid, map;
function initMap() {
	var level = [];
	for (var y = 0; y < MAP_HEIGHT; y++) {
		level.push([]);
		for (var x = 0; x < MAP_WIDTH; x++) {
			if (Math.round(Math.random() * 2) === 0) {
				level[y].push({blocked: true});
			}
			else {
				level[y].push({blocked: false});
			}
		}
	}

	level[0][0].blocked = false;
	level[98][98].blocked = false;

	grid = new Grid(level);
	console.time("time");
	map = Map(grid.nodes, grid.nodes[0][0], grid.nodes[98][98]);
	console.timeEnd("time");

	if (map.length === 0) {
		initMap();
	}
}

initMap();


// draw

var canvas = document.createElement("canvas");
canvas.height = MAP_HEIGHT * BLOCK_SIZE;
canvas.width = MAP_WIDTH * BLOCK_SIZE;
var ctx = canvas.getContext("2d");

document.body.appendChild(canvas);

ctx.clearRect(0, 0, canvas.width, canvas.height);
for (var y = 0, yl = grid.grid.length; y < yl; y++) {
	for (var x = 0, xl = grid.grid[y].length; x < xl; x++) {
		var tile = grid.grid[y][x];
		if (tile.blocked) {
			ctx.fillStyle = "black";
		}
		else if (y === 0 && x === 0) {
			ctx.fillStyle = "green";
		}
		else if (grid.nodes[y][x].closed) {
			ctx.fillStyle = "gray";
		}
		else {
			ctx.fillStyle = "white";
		}
		ctx.fillRect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

		for (var i = 0, il = map.length; i < il; i++) {
			if (map[i].x === x && map[i].y === y) {
				ctx.fillStyle = "yellow";
			}
			if (map[map.length-1].x === x && map[map.length-1].y === y) {
				ctx.fillStyle = "red";
			}
		}
		ctx.fillRect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
		ctx.strokeRect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	}
}