
const fileInput = document.getElementById('dataFileUpload');
fileInput.addEventListener('change', readJson);

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
  document.getElementById('dataFileName').innerHTML = fileName;
}

function readerCallback(data) {
  var jsonData = JSON.parse(data);
  var string = "";

  for (var i = 0; i < jsonData.mock_data.length; i++) {
    string += "<b> Time Stamp (UTC): </b>" + jsonData.mock_data[i].TimeStampDate + "<b> Altitude (m): </b>" + jsonData.mock_data[i].AltitudeQnh_meters +
              "<b> AirSpeed (knots): </b>" + jsonData.mock_data[i].AirSpeed_knots + "<b> Vertical Speed (m/s): </b>" + jsonData.mock_data[i].VerticalSpeed_ms +
              "<b> Normal Acceleration (g): </b>" + jsonData.mock_data[i].NormalAcceleration_g + "<b> Motor Power (v): </b>" + jsonData.mock_data[i].MotorPower_volts + "<br>";
  }
  document.getElementById('data').innerHTML = string;
}
