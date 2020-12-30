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

  for (var i = 0; i < jsonData.mock_data.length; i++) {
    string += "<b> Time Stamp (UTC): </b>" + jsonData.mock_data[i].TimeStampDate + "<b> Altitude (m): </b>" + jsonData.mock_data[i].AltitudeQnh_meters +
              "<b> AirSpeed (knots): </b>" + jsonData.mock_data[i].AirSpeed_knots + "<b> Vertical Speed (m/s): </b>" + jsonData.mock_data[i].VerticalSpeed_ms +
              "<b> Normal Acceleration (g): </b>" + jsonData.mock_data[i].NormalAcceleration_g + "<b> Motor Power (v): </b>" + jsonData.mock_data[i].MotorPower_volts + "<br>";
  }
  document.getElementById('data').innerHTML = string;
  createGraph(jsonData);
}

function createGraph(jsonData) {

  let timeSorted = [];
    for(var i = 0; i < jsonData.mock_data.length; i++) {
      timeSorted.push(moment.utc(jsonData.mock_data[i].TimeStampDate));
    }
    timeSorted.sort();


  var data5 = [];
  for(var j = 0; j < jsonData.mock_data.length; j++) {
    data5.push({t: timeSorted[j], y: jsonData.mock_data[j].AltitudeQnh_meters});
  }

let myChart5 = document.getElementById("myChart5").getContext('2d');

let chart5 = new Chart(myChart5, {
  type: 'line',
  data: {
    labels: timeSorted,
    datasets: [{
      label: 'Altitude Qnh (meters)',
      data: data5,
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
}
