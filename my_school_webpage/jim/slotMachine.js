function machine(){
	var canvas;
	var ctx;
	var reelOne;
	var reelTwo;
	var reelThree;
	var spins;
	var totalpayout;
	var winnings;
	var avgWin;
	var slots;
}

var setup = function(){
	machine.canvas = document.getElementById("canvas");
	machine.ctx = canvas.getContext("2d");
	
	machine.ctx.fillStyle = "#008000";
	machine.ctx.fillRect(0, 0, machine.canvas.width, machine.canvas.height);
	
	machine.ctx.fillStyle = "#000000";
	machine.ctx.font = "30px sans-serif";
	machine.ctx.fillText("Press Enter to Spin the Slots", 50, 250);
	
	machine.spins = 0;
	machine.winnings = 0;
	
	machine.reelOne = [2, 3, 0, 2, 3, 1, 0, 2, 1, 0, 1, 1, 0, 2, 2, 2, 0];
	machine.reelTwo = [1, 0, 0, 2, 3, 0, 1, 0, 2, 0, 0, 2, 0];
	machine.reelThree = [3, 2, 1, 0, 0, 2, 0, 1, 2, 1, 0, 0, 2, 0, 0];
	
	machine.slots = [];
}

window.onkeydown = function(evnt){
	evnt = evnt || window.evnt;
	if(evnt.keyCode == 13){
		spin();
		getAverageWin();
		draw();
	}
}

var spin = function(){
	var firstSlot = Math.floor(Math.random() * 17);
	var secondSlot = Math.floor(Math.random() * 13);
	var thirdSlot = Math.floor(Math.random() * 15);
	machine.totalpayout = 0;
	
	machine.slots[0] = machine.reelOne[firstSlot];
	machine.slots[3] = (machine.reelOne[(firstSlot+1)%17]);
	machine.slots[6] = (machine.reelOne[(firstSlot+2)%17]);
	machine.slots[1] = machine.reelTwo[secondSlot];
	machine.slots[4] = (machine.reelTwo[(secondSlot+1)%13]);
	machine.slots[7] = (machine.reelTwo[(secondSlot+2)%13]);
	machine.slots[2] = machine.reelThree[thirdSlot];
	machine.slots[5] = (machine.reelThree[(thirdSlot+1)%15]);
	machine.slots[8] = (machine.reelThree[(thirdSlot+2)%15]);
	
	checkLine(machine.slots[0], machine.slots[1], machine.slots[2]);
	checkLine(machine.slots[3], machine.slots[4], machine.slots[5]);
	checkLine(machine.slots[6], machine.slots[7], machine.slots[8]);
	checkLine(machine.slots[0], machine.slots[4], machine.slots[8]);
	checkLine(machine.slots[6], machine.slots[4], machine.slots[2]);
	checkLine(machine.slots[0], machine.slots[4], machine.slots[5]);
	checkLine(machine.slots[6], machine.slots[4], machine.slots[5]);
	checkLine(machine.slots[3], machine.slots[1], machine.slots[2]);
	checkLine(machine.slots[3], machine.slots[7], machine.slots[8]);
	
	machine.winnings += machine.totalpayout;
	machine.spins++;
}

var checkLine = function(slot1, slot2, slot3){

	var lineSlots = [];
	var cherries = 0;
	var grapes = 0;
	var oranges = 0;
	var wilds = 0;
	var payout = 0;
	
	lineSlots[0] = slot1;
	lineSlots[1] = slot2;
	lineSlots[2] = slot3;

	for(var n = 0; n < 3; n++){
		switch(lineSlots[n]){
			case 0:
				cherries++;
				break;
			case 1:
				grapes++;
				break;
			case 2:
				oranges++;
				break;
			case 3:
				wilds++;
				break;
			default:
		}
	}
	
	if(cherries == 3){
		payout = 5;
	} else if(cherries == 2){
		payout = 1;
	} else if(grapes == 3){
		payout = 10;
	} else if(oranges == 3){
		payout = 15;
	} else if(wilds == 3){
		payout = 100;
	}
	
	machine.totalpayout += payout;
}

var getAverageWin = function(){
	machine.avgWin = (machine.winnings/machine.spins);
}

var draw = function(){
	machine.ctx.fillStyle = "#008000";
	machine.ctx.fillRect(0, 0, machine.canvas.width, machine.canvas.height);
	
	machine.ctx.fillStyle = "#000000";
	machine.ctx.font = "30px sans-serif";
	for(var n = 0; n < 3; n++){
		for(var m = 0; m < 3; m++){
			switch(machine.slots[(n*3) + m]){
				case 0:
					machine.ctx.fillText("C", (50 + (100 * m)), (50 + (100 * n)));
					break;
				case 1:
					machine.ctx.fillText("G", (50 + (100 * m)), (50 + (100 * n)));
					break;
				case 2:
					machine.ctx.fillText("O", (50 + (100 * m)), (50 + (100 * n)));
					break;
				case 3:
					machine.ctx.fillText("W", (50 + (100 * m)), (50 + (100 * n)));
					break;
				default:
			}
		}
	}
	
	machine.ctx.fillText("payout: " + machine.totalpayout, 50, 350);
	machine.ctx.fillText("average winnings: " + machine.avgWin, 50, 450);
}

setup();