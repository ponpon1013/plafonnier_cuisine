// bouton wifi_status
var wifiStatusBouton = document.getElementById("wifi_status");
// fenetre wifi configure
var wifiConfigWindow = document.getElementById("wifi_config");
var wifiAvailable = document.getElementById("wifi_available");
var ssidEntry = document.getElementById("ssidEntry");
var ssidChoosen = "";

var red = "red";
var green = "#47f263";

// creation du prototype d'objet taskInfo
var TaskInfo = function(nom, param) {
  this.nom = nom;
  this.param = param;
};

TaskInfo.prototype.execute = function() {
  // just to have prototyp. to see if ist necessary
};

// taskWifi definitaion

//funtion to sort Wifi array by rrsi
function sort(x, y) {
  return ((x < y) ? 1 : (x > y) ? -1 : 0);
}

function sortBySsid(a, b) {
  x = a.ssid;
  y = b.ssid;
  return sort(x, y);
}

function sortByrrsi(a, b) {
  x = a.rrsi;
  y = b.rrsi;
  return sort(x, y);
}

function TaskWifi(nom, param) {
  TaskInfo.call(this, nom, param);
}


function ssidFilter(item, pos, self) {
  // we search if previous item are the same ssid
  // we start with result=0
  var result = 0;
  for (var i = 0; i < pos; i++) {
    // if same ssid is found result change to 1
    result = result || (self[i].ssid === item.ssid);
  }
  // if same ssid is found it is needed to resturn 0 else 1
  // so it is !result;
  return !result;
}

function handlechange(radioBox) {
  if (radioBox.checked) {
    var idToFind = radioBox.getAttribute("id");
    var radioBoxFather = radioBox.parentElement;
    var labelList = radioBoxFather.childNodes;
    labelList.forEach(function(element) {
      if (element.nodeName === "LABEL") {
        if (element.getAttribute("for") === idToFind) {
          var pList = element.childNodes;
          pList.forEach(function(pElement) {
            if (pElement.nodeName === "P") {
              ssidChoosen = pElement.innerHTML;
            }
          });
        }
      }
    });
    //ssidChoosen = radioBox.getAttribute("id");
    ssidEntry.innerHTML = "SSID:" + ssidChoosen;
  }
}

TaskWifi.prototype = Object.create(TaskInfo.prototype);
TaskWifi.prototype.constructeur = TaskWifi;
TaskWifi.prototype.execute = function() {
  console.log("WiFiScan treatment");
  paramFiltered = this.param.filter(ssidFilter); // remove doublons
  paramFiltered.sort(sortByrrsi); // sort by rrssi biggest to lowest
  console.log("WiFiScan filetered and ordonned:", paramFiltered);
  wifiAvailable.innerHTML = "";
  paramFiltered.forEach(function(element) {
    var checkedElement = (element.ssid === ssidChoosen) ? "checked" : "";
    var col1 = (element.rrsi > -75) ? green : red;
    var col2 = (element.rrsi > -80) ? green : red;
    var col3 = (element.rrsi > -85) ? green : red;
    var col4 = (element.rrsi > -90) ? green : red;
    var lineToAdd =
      '<input type="radio" name="wifi_selec" id=' +
      element.ssid +
      ' onchange="handlechange(this);" class="bouton-radio"' +
      checkedElement +
      '/>' +
      '<label for=' + element.ssid + ' class="label-for-check">' +
      '<p>' + element.ssid + '</p>' +
      '<svg version="1.1" viewBox="0 0 100 100" id="wifi-value1" preserveAspectRation="xMinyMax meet" class="svg-content-menu">' +
      '<g fill="none" stroke-width="4px" stroke-linecap="round">' +
      '<circle id="circle" cx="50" cy="50" r="40" stroke="#47f263" />' +
      '<g transform="translate(0,5)">' +
      '<path id="full" d="M27,32 A42,42 0 0 1 73,32" stroke=' + col1 + ' />' +
      '<path id="middle" d="M34,41 A30,30 0 0 1 66,41" stroke=' + col2 + ' />' +
      '<path id="low" d="M42,51 A16,16 0 0 1 58,51" stroke=' + col3 + ' />' +
      '<circle cx="50" cy="62" r="2" stroke=' + col4 + ' />' +
      '</g>' +
      '</g>' +
      '</svg>' +
      '</label>';
    wifiAvailable.innerHTML = wifiAvailable.innerHTML + lineToAdd;

  });

};


var TaskList = function() {
  this.tabList = [];
};

TaskList.prototype.addTask = function(task) {
  this.tabList.push(task);
};

var taskList = new TaskList();
var taskWifi = new TaskWifi("WiFiScan", []);

taskList.addTask(taskWifi);

//JSON function
function IsJsonString(str) {
  try {
    JSON.parse(str);
  } catch (e) {
    return false
  }
  return true;
}


// gestion fenetre choix WiFi


var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

connection.onopen = function() {
  connection.send('Connect ' + new Date());
};
connection.onerror = function(error) {
  console.log('WebSocket Error ', error);
};
connection.onmessage = function(e) {
  //console.log('Server: ', e.data);
  if (IsJsonString(e.data)) {
    jsonCommand = JSON.parse(e.data);
    taskList.tabList.forEach(function(y) {
      if (y.nom === jsonCommand["name"]) {
        y.param = jsonCommand["param"];
        y.execute();
      }
    });
  }
};

connection.onclose = function() {
  console.log('WebSocket connection closed');
};

wifiStatusBouton.addEventListener('click', function() {
  wifiConfigWindow.style.display = "block";
  var command2send = {
    command: {
      order: "WiFiScan",
      param: "on"
    }
  };
  var json2send = JSON.stringify(command2send);
  connection.send(json2send);
});

//closing wifi config windows
function closeWifiConfig() {
  wifiConfigWindow.style.display = "none";
  var command2send = {
    command: {
      order: "WiFiScan",
      param: "off"
    }
  };
  var json2send = JSON.stringify(command2send);
  connection.send(json2send);
}

// send WiFi config to server
function SendWifiConfig() {
  closeWifiConfig();
  var password=document.getElementById("WiFiPassword").value;
  var command2send = {
    command: {
      order: "WiFi",
      param: {
        ssid: ssidChoosen,
        password: password
      }
    }
  };
  var json2send = JSON.stringify(command2send);
  connection.send(json2send);
}
