
var makeFocused = function () {
    document.getElementById("top-focused").style.zIndex++;
    document.getElementById("top-unfocused").style.zIndex--;
}

var makeUnfocused = function () {
    document.getElementById("top-focused").style.zIndex--;
    document.getElementById("top-unfocused").style.zIndex++;
}
    
