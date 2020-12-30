const fileInput = document.getElementById('dataFileUpload');
fileInput.addEventListener('change', readJson);
var jsonData;

function readJson() {
  updateInputPara(fileInput.files[0].name);
  var reader = new FileReader();
  var file = fileInput.files[0];

  reader.onload = function() {
    readerCallback(reader.result);
  }

  reader.readAsText(file);
}

function updateInputPara(fileName) {
  var str = "File Selected: <br>" + fileName;
  document.getElementById('dataFileName').innerHTML = str;
}

function readerCallback(data) {
  jsonData = JSON.parse(data);
  var string = "";

  /*for (var i = 0; i < jsonData.mock_data.length; i++) {
    string += "<b> Time Stamp (UTC): </b>" + jsonData.mock_data[i].TimeStampDate + "<b> Altitude (m): </b>" + jsonData.mock_data[i].AltitudeQnh_meters +
              "<b> AirSpeed (knots): </b>" + jsonData.mock_data[i].AirSpeed_knots + "<b> Vertical Speed (m/s): </b>" + jsonData.mock_data[i].VerticalSpeed_ms +
              "<b> Normal Acceleration (g): </b>" + jsonData.mock_data[i].NormalAcceleration_g + "<b> Motor Power (v): </b>" + jsonData.mock_data[i].MotorPower_volts + "<br>";
  }
  document.getElementById('data').innerHTML = string; */
  createGraph(jsonData);
}

function createGraph(jsonData) {

  let timeSorted = [];
    for(var i = 0; i < jsonData.mock_data.length; i++) {
      timeSorted.push(moment.utc(jsonData.mock_data[i].TimeStampDate));
    }
    timeSorted.sort();


  var altData = [];
  for(var j = 0; j < jsonData.mock_data.length; j++) {
    altData.push({t: timeSorted[j], y: jsonData.mock_data[j].AltitudeQnh_meters});
  }

let altitudeChart = document.getElementById("altitudeChart").getContext('2d');

let altChart = new Chart(altitudeChart, {
  type: 'line',
  data: {
    labels: timeSorted,
    datasets: [{
      label: 'Altitude Qnh (meters)',
      data: altData,
      fill: false,
      borderColor: 'red',
      borderWidth: '0.5'
    }]
  },
  options: {
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true
        },
        scaleLabel: {
          display: true,
          labelString: 'AltitudeQnh (meters)'
        }
      }],
      xAxes: [{
        display: true,
        type: 'time',
      }]
    },
    legend: {
      postition: 'bottom',
      display: false
    }
  }
});

var aSpdData = [];
for(var j = 0; j < jsonData.mock_data.length; j++) {
  aSpdData.push({t: timeSorted[j], y: jsonData.mock_data[j].AirSpeed_knots});
}

let airSpeedChart = document.getElementById("airSpeedChart").getContext('2d');

let aSpdChart = new Chart(airSpeedChart, {
  type: 'line',
  data: {
    labels: timeSorted,
    datasets: [{
      label: 'AirSpeed (knots)',
      data: aSpdData,
      fill: false,
      borderColor: 'red',
      borderWidth: '0.5'
    }]
  },
  options: {
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true
        },
        scaleLabel: {
          display: true,
          labelString: 'AirSpeed (knots)'
        }
      }],
      xAxes: [{
        display: true,
        type: 'time',
      }]
    },
    legend: {
      postition: 'bottom',
      display: false
    }
  }
});

var vSpdData = [];
for(var j = 0; j < jsonData.mock_data.length; j++) {
  vSpdData.push({t: timeSorted[j], y: jsonData.mock_data[j].VerticalSpeed_ms});
}

let vertSpeedChart = document.getElementById("vertSpeedChart").getContext('2d');

let vSpdChart = new Chart(vertSpeedChart, {
  type: 'line',
  data: {
    labels: timeSorted,
    datasets: [{
      label: 'Vertical Speed (ms)',
      data: vSpdData,
      fill: false,
      borderColor: 'red',
      borderWidth: '0.5'
    }]
  },
  options: {
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true
        },
        scaleLabel: {
          display: true,
          labelString: 'Vertical Speed (ms)'
        }
      }],
      xAxes: [{
        display: true,
        type: 'time',
      }]
    },
    legend: {
      postition: 'bottom',
      display: false
    }
  }
});

}
