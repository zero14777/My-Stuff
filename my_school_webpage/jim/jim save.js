function machine(){
	var canvas;
	var ctx;
	var intervalpoints;
	var points;
	var message;
	var started;
	var Jims;
	var Trevors;
	var Sids;
}

var draw = function(){
	machine.ctx.fillStyle = "#008000";
	machine.ctx.fillRect(0, 0, machine.canvas.width, machine.canvas.height);
	
	machine.ctx.fillStyle = "#000000";
	machine.ctx.font = "20px sans-serif";
	switch(machine.message){
		case 0:
			machine.ctx.fillText("At least try you shitter!", 50, 150);
			break;
		case 1:
			machine.ctx.fillText("Don't stop clicking Jim!", 30, 250);
			break;
		case 2:
			machine.ctx.fillText("Faster Faster!", 62, 126);
			break;
		case 3:
			machine.ctx.fillText("You can do it!", 132, 124);
			break;
		case 4:
			machine.ctx.fillText("You were born for this!", 120, 350);
			break;
		case 5:
			machine.ctx.fillText("Break that mouse!", 20, 20);
			break;
		case 6:
			machine.ctx.fillText("Just a litte more!", 140, 300);
			break;
		case 7:
			machine.ctx.fillText("This is so much better than doing homework!", 20, 220);
			break;
		default:
			machine.ctx.fillText("Broke!", 50, 50);
	}
	machine.ctx.fillText("Good Boy Points: " + machine.points, 50, 450);
	machine.ctx.fillText("Shop: j-( " + machine.Jims + " jims) t-( " + machine.Trevors + " trevors) s-(" + machine.Sids +" sids)", 30, 475);
}

window.onkeydown = function(evnt){
	evnt = evnt || window.evnt;
	switch(evnt.keyCode){
		case 74:
			machine.Jims++;
			break;
		case 84:
			machine.Trevors++;
			break;
		case 83:
			machine.Sids++;
			break;
		default:
	}
}

canvas.addEventListener("mouseup", click, false);

function click(){
	if(machine.started == 0) {
		window.setInterval(function(){
			checkclicks();
		}, 1000);
		window.setInterval(function(){
			machine.points = machine.points + machine.Jims + (10 * machine.Trevors) + (100 * machine.Sids);
			draw();
		}, 500);
		machine.started = 1;
	}
	machine.points++;
	machine.intervalpoints++;
	draw();
}

function checkclicks(){
	if (machine.intervalpoints == 0) {
		machine.message = 0;
	} else if (machine.intervalpoints <= 2) {
		machine.message = 1;
	} else if (machine.intervalpoints <= 4) {
		machine.message = 2;
	} else if (machine.intervalpoints <= 6) {
		machine.message = 3;
	} else if (machine.intervalpoints <= 8) {
		machine.message = 4;
	} else if (machine.intervalpoints <= 10) {
		machine.message = 5;
	} else if (machine.intervalpoints <= 12) {
		machine.message = 6;
	} else {
		machine.message = 7;
	}
	machine.intervalpoints = 0;
}

var setup = function(){
	machine.canvas = document.getElementById("canvas");
	machine.ctx = canvas.getContext("2d");
	
	machine.ctx.fillStyle = "#008000";
	machine.ctx.fillRect(0, 0, machine.canvas.width, machine.canvas.height);
	
	machine.ctx.fillStyle = "#000000";
	machine.ctx.font = "20px sans-serif";
	machine.ctx.fillText("Click to start Peter's Ultimate Clicking Game", 50, 250);
	machine.points = 0;
	machine.message = 0;
	machine.Jims = 0;
	machine.Trevors = 0;
	machine.Sids = 0;
}

machine.started = 0;
setup();
