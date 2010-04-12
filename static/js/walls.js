function Wall(width, height) {
	this.width = width;
	this.height = height;
	this.speed = 50;

	this.segments = [
		{x: 0, y: 0, width: this.width / 3},
		{x: 0, y: this.height / 2, width: this.width / 3}
	];
	this.shift = function (delta) {
		var i, s, n = 0;
		for (i = 0; i < this.segments.length; i++) {
			s = this.segments[i];
			s.y -= delta;
			if (s.y < 0)
				n++;
		}
		if (n >= 2)
			this.segments.splice(0, n - 1);
	};
	this.extend = function (width, widthVol, movement) {
		var last = this.segments[this.segments.length - 1];
		while (last.y <= this.height) {
			var newWidth = width + (2 * Math.random() - 1) * widthVol;
			var offset = movement * (Math.random() * 2 - 1);
			var s;

			var x = last.x + this.width / 2 + offset;
			if (x + newWidth >= this.width) {
				s = {x: last.x - movement, y: last.y + 100, width: newWidth};
			} else if (x - newWidth <= 0) {
				s = {x: last.x + movement, y: last.y + 100, width: newWidth};
			} else {
				s = {x: last.x + offset, y: last.y + 100, width: newWidth};
			}
			this.segments.push(s);
			last = s;
		}
	};
	this.extend(this.width / 3, this.width / 24, this.width / 8, this.width / 16);

	var intersection = function (p, q, vx) {
		// special case for vertical line segment
		if (p.x == q.x) {
			return p.x == vx;
		}
		return p.y + ((q.y - p.y) / (q.x - p.x)) * (vx - p.x);
	};

	this.checkCollision = function (ctx, ship) {
		var lhs = ship.lhs();
		var rhs = ship.rhs();
		var i, pl, pr, ql, qr, s;
		for (i = 0; i < 2; i++) {
			s = this.segments[i];
			pl = {x: s.x - s.width / 2, y: s.y};
			pr = {x: s.x + s.width / 2, y: s.y};
			s = this.segments[i + 1];
			ql = {x: s.x - s.width / 2, y: s.y};
			qr = {x: s.x + s.width / 2, y: s.y};
			if (pl.y > ship.height || ql.y < ship.baseHeight)
				continue;

			var isec = intersection(pl, ql, lhs);
			if (isec >= ship.baseHeight && isec <= ship.height + ship.baseHeight && isec >= pl.y && isec <= ql.y) {
				ctx.strokeStyle = ctx.fillStyle = "rgb(255, 0, 0)";
				ctx.beginPath();
				ctx.arc(lhs, isec, 3, 0, 2 * Math.PI, false);
				ctx.fill();
				return true;
			}
			isec = intersection(pr, qr, rhs);
			if (isec >= ship.baseHeight && isec <= ship.height + ship.baseHeight && isec >= pr.y && isec <= qr.y) {
				ctx.strokeStyle = ctx.fillStyle = "rgb(255, 0, 0)";
				ctx.beginPath();
				ctx.arc(rhs, isec, 3, 0, 2 * Math.PI, false);
				ctx.fill();
				return true;
			}
		}
		return false;

	};

	this.draw = function (ctx) {
		var i, s;
		ctx.strokeStyle = "rgb(244, 251, 64)";
		ctx.lineJoin = "round";

		// left wall
		s = this.segments[0];
		ctx.beginPath();
		ctx.moveTo(s.x - s.width / 2, s.y);
		for (i = 1; i < this.segments.length; i++) {
			s = this.segments[i];
			ctx.lineTo(s.x - s.width / 2, s.y);
		}
		ctx.stroke();
		
		// right wall
		s = this.segments[0];
		ctx.beginPath();
		ctx.moveTo(s.x + s.width / 2, s.y);
		for (i = 1; i < this.segments.length; i++) {
			s = this.segments[i];
			ctx.lineTo(s.x + s.width / 2, s.y);
		}
		ctx.stroke();
	};
}
