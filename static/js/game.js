function Game () {
	this.gameOver = false;
	this.keepGoing = false;
	this.ctx = null;
	this.speed = 15;

	this.init = function () {
		var canvas = document.getElementById("canvas");
		if (canvas.getContext) {
			this.width = canvas.width;
			this.height = canvas.height;
			this.walls = new Wall(canvas.width, canvas.height);
			this.ship = new Ship(20, 30, 5);
			this.score = 0;

			this.ctx = canvas.getContext("2d");
			this.ctx.lineWidth = 2;
			this.ctx.fillStyle = "rgb(57, 55, 80)";
			this.ctx.strokeStyle = "rgb(244, 251, 64)";

			this.tick();
		}
	};

	this.tick = function () {
		var px, py;
		var start = (new Date()).valueOf();
		
		this.ctx.fillStyle = "rgb(57, 55, 80)";
		this.ctx.strokeStyle = "rgb(244, 251, 64)";
		
		this.ctx.clearRect(0, 0, this.width, this.height);
		this.ctx.fillRect(0, 0, this.width, this.height);
		
		this.ctx.save();
		this.ctx.setTransform(1, 0, 0, -1, this.width / 2, this.height, false);
		
		this.ctx.fillStyle = "rgb(244, 251, 64)";
		this.ship.draw(this.ctx);
		this.walls.draw(this.ctx);

		if (this.keepGoing && !this.gameOver) {
			/* Update the position */
			if (this.walls.checkCollision(this.ctx, this.ship))
				this.gameOver = true;
			this.pos += this.velocity;
			this.walls.shift(5);
			this.walls.extend(this.width / 3, this.width / 12, this.width / 6);

			this.ship.move();

			var elapsed = (new Date()).valueOf() - start;
			window.setTimeout(tickCallback, this.speed - elapsed);
		}
		this.ctx.restore();

		this.ctx.font = "12pt monospace";
		this.ctx.fillStyle = "rgb(244, 251, 64)";
		this.ctx.fillText(parseInt(this.score, 10), 10, 20);

		if (this.keepGoing) {
			var now = (new Date()).valueOf();
			this.ctx.fillText(((now - this.ship.moveTime) / 1000).toFixed(2), this.width * 0.87, 20);
		} else {
			this.ctx.fillText("0.00", this.width * 0.87, 20);
		}


		if (this.gameOver) {
			this.ctx.font = "40pt monospace";
			this.ctx.fillStyle = "rgb(255, 0, 0)";
			// TODO: get measureText working, to properly center this
			this.ctx.fillText("fail", this.width / 3, this.height / 2);
		} else {
			var bonus = this.ship.getBonus();
			if (bonus > 1)
				this.score *= bonus;
			else
				this.score += 10;
		}
	};

	this.init();
}
