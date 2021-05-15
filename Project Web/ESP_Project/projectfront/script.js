
const fileInput = document.getElementById('dataFileUpload');
fileInput.addEventListener('change', readJson);

function readJson() {
  /*var str = "File Selected: <br>" + fileInput.files[0].name;
  document.getElementById('dataFileName').innerHTML = str;

  var reader = new FileReader();
  var file = fileInput.files[0];

  reader.onload = function() {
    readData(reader.result);
  }

  reader.readAsText(file);*/
  httpGetAsync()
}

function httpGetAsync()
{
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'http://192.168.0.64:4000/app/getDB', true);

    xhr.onload = function () {
      readData(xhr.response);
    };

    xhr.send(null);
}

function readData(dataInput) {

  jsonData = JSON.parse(dataInput);

  let timeStamp = [];

  let magX = [];
  let magY = [];
  let magZ = [];

  let gyrX = [];
  let gyrY = [];
  let gyrZ = [];

  let accX = [];
  let accY = [];
  let accZ = [];

  let temp = [];
  let press = [];
  let gps = [];

  for(var i = 0; i < jsonData.length; i++) {
    timeStamp.push(moment.utc(jsonData[i].date));
  }

  for(var i = 0; i < jsonData.length; i++) {

    magX.push({t: timeStamp[i], y: jsonData[i].Magnetometer_X});
    magY.push({t: timeStamp[i], y: jsonData[i].Magnetometer_Y});
    magZ.push({t: timeStamp[i], y: jsonData[i].Magnetometer_Z});

    gyrX.push({t: timeStamp[i], y: jsonData[i].Gyroscope_X});
    gyrY.push({t: timeStamp[i], y: jsonData[i].Gyroscope_Y});
    gyrZ.push({t: timeStamp[i], y: jsonData[i].Gyroscope_Z});

    accX.push({t: timeStamp[i], y: jsonData[i].Accelerometer_X});
    accY.push({t: timeStamp[i], y: jsonData[i].Accelerometer_Y});
    accZ.push({t: timeStamp[i], y: jsonData[i].Accelerometer_Z});

    temp.push({t: timeStamp[i], y: jsonData[i].Temperature});
    press.push({t: timeStamp[i], y: jsonData[i].Pressure});
    gps.push({t: timeStamp[i], y: jsonData[i].GPS_String});
  }

  let allData = [magX, magY, magZ, gyrX, gyrY, gyrZ, accX, accY, accZ, temp, press, gps];

  createGraphs(allData);

  console.log(allData);
}

function createGraphs(data) {

  let chartElements = ['magnetometerChartX', 'magnetometerChartY', 'magnetometerChartZ','gyroscopeChartX', 'gyroscopeChartY',
                      'gyroscopeChartZ', 'accelerometerChartX', 'accelerometerChartY', 'accelerometerChartZ', 'temperatureChart', 'pressureChart', 'gpsChart']
  let chartLabels = ['Magnetometer X', 'Magnetometer Y', 'Magnetometer Z', 'gyroscope X', 'gyroscope Y', 'gyroscope Z', 
                      'Accelerometer X', 'Accelerometer Y', 'Accelerometer Z', 'Temperature (C)', 'Pressure (Pa)', 'GPS ']

  for(let i = 0; i < chartElements.length; i++) {

    if (chartElements[i] == ' magnetometerChartX' || chartElements[i] == 'gyroscopeChartX' || chartElements[i] == 'accelerometerChartY') {
      let chart1 = document.getElementById(chartElements[i]).getContext('2d');
      let chart2 = document.getElementById(chartElements[i + 1]).getContext('2d');
      let chart3 = document.getElementById(chartElements[i + 2]).getContext('2d');

    new Chart(chart1, {
      type: 'line',
      data: chartData(chartLabels[i], data[0], data[i]),
      options: chartOptions(chartLabels[i])
    });

    new Chart(chart2, {
      type: 'line',
      data: chartData(chartLabels[i + 1], data[0], data[i+1]),
      options: chartOptions(chartLabels[i + 1])
    });

    new Chart(chart3, {
      type: 'line',
      data: chartData(chartLabels[i + 2], data[0], data[i+2]),
      options: chartOptions(chartLabels[i + 2])
    });
    
    }
    else {
      let chart1 = document.getElementById(chartElements[i]).getContext('2d');

      new Chart(chart1, {
        type: 'line',
        data: chartData(chartLabels[i], data[0], data[i]),
        options: chartOptions(chartLabels[i])
      });
    }
  }
}

var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (this.classList.contains("active")){
      content.style.maxHeight = content.scrollHeight + "px";
    } else {
      content.style.maxHeight = 0;
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
