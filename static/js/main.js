var game = null;

function toggleGame() {
	if (game && game.keepGoing) {
		game.keepGoing = false;
	} else {
		if (!game)
			game = new Game();
		game.keepGoing = true;
		game.tick();
	}
}

function tickCallback () {
	if (game) {
		game.tick();
		if (game.gameOver)
			game = null;
	}
}


function init() {
	game = new Game();
}

/* Key-handling code. This effectively overrides the normal onkeydown system so
 * that we get accelerate the ship at a regular interval (100 ms) rather than
 * however often the user's computer decided it wanted to fire repeate key
 * presses at.
 */
var accelId = null;
var leftDown = false;
var rightDown = false;

function accelerateShip(direction) {
	if (game) {
		game.ship.accelerate(direction);
		accelId = setTimeout(function() { accelerateShip(direction); }, 100);
	}
}

document.onkeydown = function (e) {
	if (game && game.keepGoing && !game.gameOver) {
		if (e.which == 37 && !leftDown) {
			leftDown = true;
			accelerateShip(-1);
		} else if (e.which == 39 && !rightDown) {
			rightDown = true;
			accelerateShip(1);
		}
	}
	if (e.which == 32) {
		// disable page-down on spacebar
		if ((e.keyCode == 32) && (!e.shiftKey) && e.preventDefault)
			e.preventDefault();

		if (!game || game.gameOver)
			game = new Game();
		if (!game.keepGoing) {
			game.keepGoing = true;
			game.ship.moveTime = (new Date()).valueOf();
			game.tick();
		}
	}
}

document.onkeyup = function (e) {

	var clear = false;
	if (e.which == 37) {
		leftDown = false;
		clear = true;
	} else if (e.which == 39) {
		rightDown = false;
		clear = true;
	}
	if (clear && accelId) {
		clearTimeout(accelId);
		accelId = null;
	}
}
