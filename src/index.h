#include <Arduino.h>
static const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
<h1>Aqua ESP on Work</h1>
Current Time : <span id="BoardTime">NA</span>
</div>

<br>

<div>
  Lamp 1 : 
	<button type="button" onclick="sendData(1,1)">ON</button>
	<button type="button" onclick="sendData(1,0)">OFF</button>
</div>
<br>
<div>
  Lamp 2 : 
	<button type="button" onclick="sendData(2,1)">ON</button>
	<button type="button" onclick="sendData(2,0)">OFF</button>
</div>
<br>
<div>
  Lamp 3 : 
	<button type="button" onclick="sendData(3,1)">ON</button>
	<button type="button" onclick="sendData(3,0)">OFF</button>
</div>
<br>
<div>
  Fun : 
  <input id="servo" type="range" min="0" max="100" onchange="sendFun(this.value)" value="0">
	<button type="button" onclick="sendData(5,1)">ON</button>
	<button type="button" onclick="sendData(5,0)">OFF</button>
</div>

<br>

<div>
	Lamp 1 State : <span id="Lamp1State">NA</span>
  <br>
  Lamp 2 State : <span id="Lamp2State">NA</span>
  <br>
  Lamp 3 State : <span id="Lamp3State">NA</span>
  <br>
  Fun State : <span id="FunState">NA</span>
  <br>
</div>

<div>
	Controller humidity : <span id="BoardHumi">NA</span>
  <br>
  Controller temperature : <span id="BoardTemp">NA</span>
  <br>
  Ambient temperature : <span id="AmbientTemp">NA</span>
  <br>
  Epra temperature : <span id="EpraTemp">NA</span>
  <br>
  Water temperature : <span id="WaterTemp">NA</span>
</div>

<script>
function sendData(lamp,led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      if (lamp==1) document.getElementById("Lamp1State").innerHTML = this.responseText;
      if (lamp==2) document.getElementById("Lamp2State").innerHTML = this.responseText;
      if (lamp==3) document.getElementById("Lamp3State").innerHTML = this.responseText;
      if (lamp==5) document.getElementById("FunState").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setLED?LED="+lamp+"&LEDstate="+led, true);
  xhttp.send();
}

function sendFun(value) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("FunState").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setFUN?FUNstate="+value, true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("BoardHumi").innerHTML = this.responseXML.getElementsByTagName('BoardHumi')[0].childNodes[0].nodeValue;
      document.getElementById("BoardTemp").innerHTML = this.responseXML.getElementsByTagName('BoardTemp')[0].childNodes[0].nodeValue;
      document.getElementById("AmbientTemp").innerHTML = this.responseXML.getElementsByTagName('AmbientTemp')[0].childNodes[0].nodeValue;
      document.getElementById("EpraTemp").innerHTML = this.responseXML.getElementsByTagName('EpraTemp')[0].childNodes[0].nodeValue;
      document.getElementById("WaterTemp").innerHTML = this.responseXML.getElementsByTagName('WaterTemp')[0].childNodes[0].nodeValue;
      document.getElementById("BoardTime").innerHTML = this.responseXML.getElementsByTagName('BoardTime')[0].childNodes[0].nodeValue;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
</script>

<br><br>
<a href="/onewire">OneWire</a>

</body>
</html>
)=====";