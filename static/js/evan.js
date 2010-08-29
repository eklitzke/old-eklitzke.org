$(document).ready(function () {
	var x1, y1, bdx, bdy;
	var bd = document.getElementById('bd');
	var move = function (e) {
		var dx = e.pageX - x1;
		var dy = e.pageY - y1;
		bd.style.left = (bdx + dx) + "px";
		bd.style.top = (bdy + dy) + "px";
	}

	$(bd).mousedown(function (e) {
		x1 = e.pageX;
		y1 = e.pageY;
		bdx = parseInt(bd.style.left) || 0;
		bdy = parseInt(bd.style.top) || 0;
		$(document).mousemove(move);
	});
	$(bd).mouseup(function (e) {
		$(document).unbind('mousemove', move);
	});
});
