function Ship (width, height, baseHeight) {
	this.width = width;
	this.height = height;
	this.baseHeight = baseHeight;
	this.pos = 0;
	this.velocity = 0;
	this.hasMoved = false;

	this.moveTime = (new Date()).valueOf();
	this.bonus = 0;

	this.draw = function (ctx) {
		var w = this.width / 2;

		ctx.beginPath();
		ctx.moveTo(this.pos - w, this.baseHeight + 3);
		ctx.lineTo(this.pos - w, this.baseHeight + this.height - 3);
		ctx.lineTo(this.pos + w, this.baseHeight + this.height - 3);
		ctx.lineTo(this.pos + w, this.baseHeight + 3);
		ctx.closePath();
		ctx.fill();

		ctx.arc(this.pos - w + 3, this.baseHeight + 3, 3, 0, 2 * Math.PI);
		ctx.arc(this.pos - w + 3, this.baseHeight + this.height - 3, 3, 0, 2 * Math.PI);
		ctx.arc(this.pos + w - 3, this.baseHeight + 3, 3, 0, 2 * Math.PI);
		ctx.arc(this.pos + w - 3, this.baseHeight + this.height - 3, 3, 0, 2 * Math.PI);
		ctx.fill();
	};

	this.lhs = function () {
		return this.pos - this.width / 2;
	};
	this.rhs = function () {
		return this.pos + this.width / 2;
	};
	this.accelerate = function (direction) {
		this.moveTime = (new Date()).valueOf();
		this.velocity += direction * (0.20 + Math.random() * 0.5)
	};
	this.move = function () {
		this.pos += this.velocity;
	};
	this.getBonus = function () {
		var now = (new Date()).valueOf();
		if ((now - this.moveTime) / 1000 - this.bonus >= 1) {
			this.bonus++;
			switch (this.bonus) {
			case 1:
				return 1.1;
			case 2:
				return 1.25;
			default:
				return 2;
			}
		}
		return 1;
	}
}
