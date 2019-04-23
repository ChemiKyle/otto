function ajaxRequest(path, type="GET") {
    var xhttp = new XMLHttpRequest();

    xhttp.open(type, path, true); // pass value of button to python function
    xhttp.send();
}

function sendOnkyoCommand(e) {
    ajaxRequest(`onkyo/${e.value}`);
}


function slideCommand(e) {
    var direction;
    e.value > 0 ? direction = "UP" : direction = "DOWN";

    console.log(direction);
    ajaxRequest(`/onkyo/MVL${direction}`);
    // slideCommand(e); // Yikes
}

function returnSlider() {
    document.getElementById('vol_slider').value = 0;
}


// Roku
function sendRoku(e) {
    // Cross origin request blocked :(
    console.log("sending TV");
    console.log(e.name);
    var tv_ip = '192.168.2.6';
    var xhttp = new XMLHttpRequest();
    xhttp.open(`${e.type}`, `http://${tv_ip}:8060/${e.value}`, true);
    xhttp.send();
    console.log(xhttp.responseText);
}


function sendRokuP(e) {
    if (e.id == "text-input") { e.value = "$~".concat(e.value); }

    ajaxRequest(`roku/${e.value}`);

    if (e.id == "text-input") { e.value = e.value.substring(2); }
}

// VLC
function sendVLCCommand(e) {
    // Cross origin request blocked :(

    var xhttp = new XMLHttpRequest();

    xhttp.open("GET", `http://192.168.2.5:8080/requests/status.xml?command=${e.value}`, true, null, "password");
    xhttp.send();
    console.log("sent");
    console.log(xhttp.responseText);
}

function sendVLCCommandP(e) {
    if ( e.value.includes('seek&val=') ) { e.value += document.getElementsByName('seek-value')[0].value; }
    ajaxRequest(`vlc/${e.value}`);

    if ( e.value.includes('seek&val=') ) { e.value = e.value.substring(0, e.value.length - 2); }

}

// MQTT
function sendMQTT(e) {
    var obj = {topic: e.name, cmd: e.value};

    ajaxRequest(`mqtt/${JSON.stringify(obj)}`);
}
