function sendOnkyoCommand(e) {

    var xhttp = new XMLHttpRequest();

    xhttp.open("GET", `onkyo/${e.value}`, true); // pass value of button to python function
    xhttp.send();
}


function slideCommand(e) {
    // console.log("old");

    // console.log(volume);
    // console.log("new");

    var xhttp = new XMLHttpRequest();

    var val = e.value;

    var direction;
    val > 0 ? direction = "UP" : direction = "DOWN";
    // volume = val;

    console.log(direction);

    // TODO: make this generic, accept path, reuse in others
    function send(){
        xhttp.open("GET", `/onkyo/MVL${direction}`, true); // pass value of button to python function
        xhttp.send();
    }
    send(direction);
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

    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", `roku/${e.value}`, true);
    xhttp.send();

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
    // Cross origin request blocked :(

    var xhttp = new XMLHttpRequest();

    xhttp.open("GET", `vlc/${e.value}`, true);
    xhttp.send();
}

function sendMQTT(e) {

    var xhttp = new XMLHttpRequest();

    var obj = {topic: e.name, cmd: e.value};

    xhttp.open("GET", `mqtt/${JSON.stringify(obj)}`, true); // pass value of button to python function
    xhttp.send();
}
