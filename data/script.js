var _activePage = ""

function buttonNavBarClicked(newValue, elmnt) {
    var children = document.getElementById("pageContainer").children
    Array.from(children).forEach(child => {
        child.style.display = "none"
    });

    children = document.getElementById("navbar").children
    Array.from(children).forEach(child => {
        child.classList = ""
    });
    elmnt.classList = "active"
    document.getElementById(newValue).style.display = "block"
    _activePage = newValue;
}


////////////////////////////////////////////// AUDIO /////////////////////////////////////////

/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */

var sourceIDS = new Array("Bluetooth", "SD card", "Webradio");
var outputIDS = new Array("I2SStream");

var selectedOutput = outputIDS[0];
var selectedSource = sourceIDS[0];

var pendingOutput = outputIDS[0];
var pendingSource = sourceIDS[0];

function AudioDropdownButton_Click(elem) {
    if(elem.parentElement.children[1].children.length > 0) elem.parentElement.children[1].classList.toggle("show");
}

function AudioDropdownContent_MouseLeave(elem) {
    elem.lastElementChild.classList.remove("show");
}

function AudioDropdownContentButton_Click(gev, mev, elem) {
    // var s = document.getElementById();
    
    if(elem == null) {
        elem = gev.target
    }

    if(elem === null) return;
    elem.parentElement.classList.remove("show");
    var type = elem.parentElement.parentElement.id;
    if( type === "AudioSourceDropdown" ) {
        pendingSource = elem.id;
    } else if( type === "AudioOutputDropdown" ) {
        pendingOutput = elem.id;
    }
    console.log("Selected : " + type + " | " + elem.id);
    UpdateDropdownsAndUpdateButton()
}

function UpdateDropdownsAndUpdateButton() {

    document.getElementById("AudioSourceDropdownButton").innerHTML = pendingSource;
    document.getElementById("AudioOutputDropdownButton").innerHTML = pendingOutput;

    var AudioSourceDropdownContent = document.getElementById("AudioSourceDropdownContent");

    while (AudioSourceDropdownContent.lastElementChild) {
        AudioSourceDropdownContent.removeChild(AudioSourceDropdownContent.lastElementChild);
    }

    sourceIDS.forEach((id) => {
        if(id !== pendingSource) {
            var button = document.createElement("button");
            button.id = id;
            button.addEventListener("click", AudioDropdownContentButton_Click, button)
            button.innerHTML = id;
            AudioSourceDropdownContent.appendChild(button);
        }
    });

    var AudioOutputDropdownContent = document.getElementById("AudioOutputDropdownContent");

    while (AudioOutputDropdownContent.lastElementChild) {
        AudioOutputDropdownContent.removeChild(AudioOutputDropdownContent.lastElementChild);
    }

    outputIDS.forEach((id) => {
        if(id !== pendingOutput) {
            var button = document.createElement("button");
            button.id = id;
            button.addEventListener("click", AudioDropdownContentButton_Click, button)
            button.innerHTML = id;
            AudioOutputDropdownContent.appendChild(button);
        }
    });

    if(pendingOutput == selectedOutput && pendingSource == selectedSource) 
    {
        document.getElementById("UpdateAudioInput-OutputButton").classList.remove("show");
    } else {
        document.getElementById("UpdateAudioInput-OutputButton").classList.add("show");
    }
}

UpdateDropdownsAndUpdateButton()

// Close the dropdown menu if the user clicks outside of it
// window.onclick = function(event) {
//     if (!event.target.matches('.AudioDropdownButton')) {
//         var dropdowns = document.getElementsByClassName("AudioDropdownContent");
//         var i;
//         for (i = 0; i < dropdowns.length; i++) {
//             var openDropdown = dropdowns[i];
//             if (openDropdown.classList.contains('show')) {
//                 openDropdown.classList.remove('show');
//             }
//         }
//     }
// }



//////////////////////////////////////////// MONITORING //////////////////////////////////////

setInterval(
    function getData()
    {
        var xhttp = new XMLHttpRequest()
        xhttp.timeout = 1000;

        xhttp.onreadystatechange = function()
            {
                if(this.readyState == 4)
                {
                    if(this.status != 200) 
                    {
                        console.log("Wifi data get return a "+this.status+" code.")
                        UpdateConnectionStatus(false)
                        UpdateData_system(undefined)
                        UpdateData_bluetooth(undefined)
                        UpdateData_wifi(undefined)
                        return
                    }
                    var result = JSON.parse(this.responseText)
                    console.log(result)
                    UpdateConnectionStatus(true)
                    UpdateData_system(result.System)
                    UpdateData_bluetooth(result.Bluetooth)
                    UpdateData_wifi(result.Wifi)
                }
            }
        xhttp.open("GET", "Data", true)
        xhttp.send()
    }, 100000000000000
)

function UpdateConnectionStatus(status) {
    var connectionStatus = document.getElementById("ConnectionStatus")
    if(status) {
        connectionStatus.style.setProperty("--ConnectionStatusColor", "green")
        connectionStatus.innerHTML = "Connected"
        return
    }
    connectionStatus.style.setProperty("--ConnectionStatusColor", "red")
    connectionStatus.innerHTML = "Disconnected"
}

function UpdateData_system(data) {
    document.getElementById("System_model").innerHTML = (data && data.model) ? data.model : "Unknown"
    document.getElementById("System_cores").innerHTML = (data && data.cores) ? data.cores : "Unknown"
    document.getElementById("System_features").innerHTML = (data && data.features) ? data.features : "Unknown"
    document.getElementById("System_full_revision").innerHTML = (data && data.full_revision) ? data.full_revision : "Unknown"
    document.getElementById("System_revision").innerHTML = (data && data.revision) ? data.revision : "Unknown"
}

function UpdateData_bluetooth(data) {
    UpdateBluetoothStatus( (data && data.status != undefined) ? data.status : -1)
    document.getElementById("Bluetooth_peerName").innerHTML = (data && data.peerName) ? data.peerName : "Unknown"
}

function UpdateBluetoothStatus(status) {
    var bluetooth_status = document.getElementById("Bluetooth_status")
    console.log(status)
    switch (status) {
        case 0:
            bluetooth_status.innerHTML = "Disconnected"
            bluetooth_status.style.color = "red"
            break;
        case 1:
            bluetooth_status.innerHTML = "Connecting"
            bluetooth_status.style.color = "orange"
            break;
        case 2:
            bluetooth_status.innerHTML = "Connected"
            bluetooth_status.style.color = "green"
            break;
        case 3:
            bluetooth_status.innerHTML = "Disconnecting"
            bluetooth_status.style.color = "orange"
            break;
        default:
            bluetooth_status.innerHTML = "Unknown"
            bluetooth_status.style.color = "var(--text-main)"
            break;
    }
}

function UpdateData_wifi(data) {
    WifiStatusUpdate( (data && data.status != undefined) ? data.status : "-1" );
    document.getElementById("Wifi_ssid").innerHTML = (data && data.ssid) ? data.ssid : "Unknown"
    document.getElementById("Wifi_ip").innerHTML = (data && data.ip) ? data.ip : "Unknown"
    document.getElementById("Wifi_mac").innerHTML = (data && data.mac) ? data.mac : "Unknown"
    document.getElementById("Wifi_subnetMask").innerHTML = (data && data.subnetMask) ? data.subnetMask : "Unknown"
    document.getElementById("Wifi_dns").innerHTML = (data && data.dns) ? data.dns : "Unknown"
}

function WifiStatusUpdate(status) {
    var wifi_status = document.getElementById("Wifi_status")
    switch (status) {
        case 255:
            wifi_status.innerHTML = "No shield"
            wifi_status.style.color = "var(--text-main)"
            break;
        case 1:
            wifi_status.innerHTML = "Idle"
            wifi_status.style.color = "var(--text-main)"
            break;
        case 2:
            wifi_status.innerHTML = "No SSID"
            wifi_status.style.color = "red"
            break;
        case 3:
            wifi_status.innerHTML = "Connected"
            wifi_status.style.color = "green"
            break;
        case 4:
            wifi_status.innerHTML = "Connexion failed"
            wifi_status.style.color = "red"
            break;
        case 5:
            wifi_status.innerHTML = "Connexion lost"
            wifi_status.style.color = "red"
            break;
        case 6:
            wifi_status.innerHTML = "Disconnected"
            wifi_status.style.color = "var(--text-main)"
            break;
        default:
            wifi_status.innerHTML = "Unknown"
            wifi_status.style.color = "var(--text-main)"
            break;
    }
}