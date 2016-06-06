// This code implements a modified version of the A-Star pathing algorithm
// for entities defined by the impactJS game library. The A-star algorithm
// is a breadth first search style algorithm that uses a calculated score
// to determine the shortest path once the destination is reached.

ig.module('game.aStar')
.requires()
.defines(function() {
	
	//A Constructor for node objects that will be used to keep track of potential paths.
	node = function (x, y, gScore, fScore, prev){
		this.x = x;
		this.y = y;
		this.gScore = gScore;
		this.fScore = fScore;
		this.prev = prev;
	};

	//The main path finding function. Accepts a specific entity "pathEntity" whose path
	//will be calculated and an array of entities "entities" filled with all of the entities
	//that are to be pathed around.
	pathFinding = function (pathEntity, entities){

		//Check to see if one of the entities to be avoided has moved onto the destination.
		//If either is true calculate a new detination for the entity.
		if(collisionCheck(pathEntity.destX, pathEntity.destY, pathEntity, entities)){
			pathEntity.findDestination();
		}

		var start = new node(pathEntity.pos.x, pathEntity.pos.y, 0, 
						fScore(pathEntity, 0, pathEntity.pos.x, pathEntity.pos.y), null);
		var nodeDist = 10;
		var closedSet = new Array(); //nodes already looked at.
		var openSet = new Array(); //nodes to be searched from.
		var path = new Array();
		openSet.push(start); //start searching from current position

		while(true){
			var current;
			var minFScore;
			var currSpot;
			
			//grabs the node in the open set with the lowest F score.
			//F score determines how expensive a path is needed to get to
			//that point.
			for(var i = 0; i < openSet.length; i++){
				if(i === 0){
					tempFScore = openSet[i].fScore;
					current = openSet[i];
					currSpot = i;
				}
				if(fScore(pathEntity, openSet[i].gScore, openSet[i].x, openSet[i].y) < tempFScore){
					tempFScore = openSet[i].fScore;
					current = openSet[i];
					currSpot = i;
				}
			}

			//If the node we are looking at is close enough to destination we just
			//attach a node with the destinations location to the end of the path
			//we found up to this point.
			if (Math.abs(current.x - pathEntity.destX) <= nodeDist,
				Math.abs(current.y - pathEntity.destY) <= nodeDist){
				var lastNode = new node(pathEntity.destX, pathEntity.destY, 0, 0, current);
				var last = current;
				while(current){
					path.splice(0, 0, current);
					current = current.prev;
					delete last.prev;
					last = current;
				}
				return path;
			}
			
			//If there are no viable nodes to path to return a path to current location.
			if(openSet.length === 0){
				var noPath = [start];
				return noPath;
			}

			openSet.splice(currSpot, 1);
			closedSet.push(current); //We mark the current node has been looked at.

			//Map out all potential new nodes surrounding our current node.
			for(var x = -1 * nodeDist; x <= nodeDist; x += nodeDist){
				for(var y = -1 * nodeDist; y <= nodeDist; y += nodeDist){
					var skip = false;
					var tempX = current.x + x;
					var tempY = current.y + y;
					var tempGScore;
					//Diagonal nodes get a higher g score since they represent
					//a longer position change. g score also represents the length
					//of a path.
					if(x != 0 && y != 0){
						tempGScore = current.gScore + 14;
					} else {
						tempGScore = current.gScore + 10;
					}
					
					//If the node position overlaps with an entity it is not made.
					if(collisionCheck(tempX, tempY, pathEntity, entities)){
						continue;
					}

					//Heavily tax pathing too close to the player to avoid enemies
					//crashing into the player. 
					//
					//This should work instead of for loop, but had issues with bugs
					//due to this in the past.
					//
					//  if(Math.sqrt(Math.pow((current.x + pathEntity.size.x - this.game.player.pos.x + (this.game.player.size.x/2)), 2) +
					//		Math.pow((current.y + pathEntity.size.y - this.game.player.pos.y + (this.game.player.size.y/2)), 2)) < 
					//		(this.game.player.moveRadius + pathEntity.moveRadius)){
					//		tempGScore += 60000;
					//	}
					for(var i = 0; i < entities.length; i++){
						if(pathEntity === entities[i] ||
							entities[i].moveRadius === 0){
							continue;
						}
						if(Math.sqrt(Math.pow((current.x + pathEntity.size.x - entities[i].pos.x + (entities[i].size.x/2)), 2) +
							Math.pow((current.y + pathEntity.size.y - entities[i].pos.y + (entities[i].size.y/2)), 2)) < 
							(entities[i].moveRadius + pathEntity.moveRadius)){
							if(entities[i].name === "PLAYER"){
								tempGScore += 60000;
								break;
							}
						}
					}
					
					//If this node represents a faster path than a previous node
					//to that spot, then update the path and gscore of the node.
					for(var n = 0; n < closedSet.length; n++){
						if(tempX === closedSet[n].x &&
							tempY === closedSet[n].y){
							if(tempGScore < closedSet[n].gScore){
								closedSet[n].gScore = tempGScore;
								closedSet[n].prev = current;
							}
							skip = true;
							break;
						}
					}
					if(skip){
						continue;
					}
					
					//If this node represents a faster path than a not yet
					//looked at node to the same spot, then update the
					//path and gscore of the node.
					for(var z = 0; z < openSet.length; z++){
						if(openSet[z].x == tempX &&
							openSet[z].y == tempY){
							if(tempGScore < openSet[z].gScore){
								openSet[z].gScore = tempGScore;
								openSet[z].prev = current;
							}
							skip = true;
							break;
						}
					}
					if(skip){
						continue;
					}
					
					openSet.push(new node(tempX, tempY, tempGScore, 
					fScore(pathEntity, tempGScore, tempX, tempY), current));
				}
			}
		}
	};

	//Loops through a array of entities and checks to see if the given "pathEntity"
	//would collide with any if it was at the positions currX and currY. Used predict
	//as well as check collisions. Returns true/false.
	collisionCheck = function (currX, currY, pathEntity, entities){
		collided = false;
		for(var i = 0; i < entities.length; i++){
			if(pathEntity === entities[i] ||
				entities[i].moveRadius === 0){
				continue;
			}
			if(Math.sqrt(Math.pow((currX + pathEntity.size.x - entities[i].pos.x - (entities[i].size.x/2)), 2) +
				Math.pow((currY + pathEntity.size.y - entities[i].pos.y - (entities[i].size.y/2)), 2)) < 
				(entities[i].pathRadius + pathEntity.pathRadius)){
				collided = true;
				break;
			}
		}

		if(collided){
			return true;
		} else {
			return false;
		}
	};

	fScore = function (pathEntity, gScore, currX, currY){
		var fScore = gScore + Math.abs((currX - pathEntity.destX) * 10) +
				Math.abs((currY - pathEntity.destY) * 10);
		return fScore;
	};
});
