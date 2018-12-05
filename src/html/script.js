// gestion fenetre choix WiFi

// bouton wifi_status
var wifiStatusBouton=document.getElementById("wifi_status");
// fenetre wifi configure
var wifiConfigWindow=document.getElementById("wifi_config");

var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

connection.onopen = function () {
  connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
};
connection.onclose = function () {
  console.log('WebSocket connection closed');
};

wifiStatusBouton.addEventListener('click',function(){
    wifiConfigWindow.style.display="block";
    connection.send("WiFi scan ON");
});

//closing wifi config windows
function closeWifiConfig(){
  wifiConfigWindow.style.display="none";
  connection.send("WiFi scan OFF");
}
