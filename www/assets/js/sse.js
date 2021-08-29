document.addEventListener('DOMContentLoaded', loadWindow, false)

// function loadWindow () {
//   Array.from(document.getElementsByTagName('button')).forEach(function (value, i, col) {
//     col[i].onclick = function (e) { mode(e.target.id) }
//   })}


 // window.addEventListener('load', loadWindow, false)
ws = new WebSocket("ws://localhost:9090")
console.log("initialized websocket")
ws.onmessage = function(evt) {
     console.log(evt.data);
}
ws.onopen = function() {
	console.log("connected");
}
ws.onclose = function() {
	console.log("closed websocket");
}

function mode (btnID) {
  ws.send(btnID)
}

  
  
function loadWindow () {
  Array.from(document.getElementsByTagName('button')).forEach(function (value, i, col) {
    col[i].onclick = function (e) { mode(e.target.id) }
    })
}
  




