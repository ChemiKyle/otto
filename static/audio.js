function sendCommand(e) {

    var xhttp = new XMLHttpRequest();

    xhttp.open("GET", `vol_change/${e.value}`, true); // pass value of button to python function
    xhttp.send();
}


function slideCommand(e) {
    console.log("old");

    console.log(volume);
    console.log("new");

    var xhttp = new XMLHttpRequest();

    var val = e.value;

    var direction;
    val > 0 ? direction = "UP" : direction = "DOWN";
    // volume = val;

    console.log(direction);

    function send(){
        xhttp.open("GET", `vol_change/MVL${direction}`, true); // pass value of button to python function
        xhttp.send();
    }
    send(direction);
    // slideCommand(e); // Yikes
}

function returnSlider() {
    document.getElementById('vol_slider').value = 0;
}


function sendMQTT(e) {

    var xhttp = new XMLHttpRequest();

    xhttp.open("GET", `mqtt/${e.value}`, true); // pass value of button to python function
    xhttp.send();
}

function sendRoku(e) {
    console.log("sending TV");
    console.log(e.name);
    var tv_ip = '192.168.2.6';
    var xhttp = new XMLHttpRequest();
    xhttp.open(`${e.type}`, `http://${tv_ip}:8060/${e.value}`, true);
    xhttp.send();
    console.log(xhttp.responseText);
}


function sendRokuP(e) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", `roku/${e.value}`, true);
    xhttp.send();
}
