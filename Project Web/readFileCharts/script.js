const fileInput = document.getElementById('dataFileUpload');
fileInput.addEventListener('change', readJson);

function readJson() {
  var str = "File Selected: <br>" + fileInput.files[0].name;
  document.getElementById('dataFileName').innerHTML = str;

  var reader = new FileReader();
  var file = fileInput.files[0];

  reader.onload = function() {
    readData(reader.result);
  }

  reader.readAsText(file);
}

function readData(dataInput) {

  jsonData = JSON.parse(dataInput);

  // Time Stamp dates
  let timeSorted = [];
  let altData = [];
  let aSpdData = [];
  let vSpdData = [];
  let nAccelData = [];
  let mPowerData = [];

  for(var i = 0; i < jsonData.mock_data.length; i++) {
    timeSorted.push(moment.utc(jsonData.mock_data[i].TimeStampDate));
  }
    timeSorted.sort();

  for(var i = 0; i < jsonData.mock_data.length; i++) {

    altData.push({t: timeSorted[i], y: jsonData.mock_data[i].AltitudeQnh_meters});
    aSpdData.push({t: timeSorted[i], y: jsonData.mock_data[i].AirSpeed_knots});
    vSpdData.push({t: timeSorted[i], y: jsonData.mock_data[i].VerticalSpeed_ms});
    nAccelData.push({t: timeSorted[i], y: jsonData.mock_data[i].NormalAcceleration_g});
    mPowerData.push({t: timeSorted[i], y: jsonData.mock_data[i].MotorPower_volts});
  }

  let allData = [timeSorted, altData, aSpdData, vSpdData, nAccelData, mPowerData];

  createGraphs(allData);
}

function createGraphs(data) {

  let chartElements = ['altitudeChart','airSpeedChart', 'vertSpeedChart', 'nAccelChart', 'mPowerChart']
  let chartLabels = ['Altitude Qnh (meters)', 'Air Speed (knots)', 'VerticalSpeed (ms)', 'Normal Acceleration (g)', 'Motor Power (volts)']

  for(let i = 0; i < chartElements.length; i++) {
    let chart = document.getElementById(chartElements[i]).getContext('2d');

    let myChart = new Chart(chart, {
      type: 'line',
      data: chartData(chartLabels[i], data[0], data[i+1]),
      options: chartOptions(chartLabels[i])
    });
  }

}

/*var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.display === "block") {
      content.style.display = "none";
    } else {
      content.style.display = "block";
    }
  }); */

var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.maxHeight){
      content.style.maxHeight = null;
    } else {
      content.style.maxHeight = content.scrollHeight + "px";
    }
  });
}

function chartOptions(label) {

  var chOptions = {
    maintainAspectRatio: false,
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true
        },
        scaleLabel: {
          display: true,
          labelString: label
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

  return chOptions;
}

function chartData(label, x, y) {
  var chData = {
    labels: x,
    datasets: [{
      label: label,
      data: y,
      fill: false,
      borderColor: 'red',
      borderWidth: '0.5'
    }]
  }
  return chData;
}
