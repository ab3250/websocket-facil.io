document.addEventListener('DOMContentLoaded', loadWindow, false)
let ctx

 ws = new WebSocket("ws://localhost:8080")
 console.log("initialized websocket")

 ws.onmessage = function(evt) {
  const d = JSON.parse(evt.data) 
    
    d.data.length < 200 ? displayDeck(d.data) : fillBox(d.data)
 }

 ws.onopen = function() {
     console.log("connected")
 }

 ws.onclose = function() {
     console.log("closed websocket")
 }

 function sleep(milliseconds) {
    var start = new Date().getTime();
    for (var i = 0; i < 1e7; i++) {
      if ((new Date().getTime() - start) > milliseconds){
        break;
      }
    }
  }

function displayDeck(arry){
    arry.forEach((value, index) => {
        document.getElementById('c' + index.toString()).src='./assets/images/' + value + '.gif'
    })
}

function n(num, len = 2) {
    return `${num}`.padStart(len, '0');
}

function shuffle(array) {
var m = array.length, t, i;

// While there remain elements to shuffle…
while (m) {

    // Pick a remaining element…
    i = Math.floor(Math.random() * m--);

    // And swap it with the current element.
    t = array[m];
    array[m] = array[i];
    array[i] = t;
}
return array;
}

function mode (btnID) {
    ws.send(btnID)
}

function fillBox(array){

  const   CanvasWidth = 800,
            CanvasHeight = 400,
            CanvasColumns = 100,
            CanvasRows = 50,
            boxWidth = CanvasWidth / CanvasColumns,
            boxHeight = CanvasHeight / CanvasRows
  let index = 0;
    for(r=0;r<CanvasRows;r++){
      for(c=0;c<CanvasColumns;c++){
        let cStart = boxWidth * c,
          cStop = cStart + boxWidth,
          rStart = boxHeight * r
          rStop = rStart + boxHeight
        ctx.fillStyle = array[index] === "0" ? 'green' : 'red'
        ctx.fillRect(cStart,rStart, cStop, rStop)
        index++
      }
    }
 }

function loadWindow (){
  const c = document.getElementById("myCanvas")
  ctx = c.getContext("2d")

  Array.from(document.getElementsByTagName('button')).forEach(function (value, i, col) {
    col[i].onclick = function (e) { mode(e.target.id) }
  })
}
