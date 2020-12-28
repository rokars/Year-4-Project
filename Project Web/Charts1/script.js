
let labels5 = ['TimeStampDate'];
  //var d = new Date(year, month, day, hours, minutes, seconds, milliseconds);
let labels6 = [
    { x: new Date(2020, 11, 10, 13, 12, 00)},
    { x: new Date(2020, 11, 10, 13, 12, 01)},
    { x: new Date(2020, 11, 10, 13, 12, 02)},
    { x: new Date(2020, 11, 10, 13, 12, 03)},
    { x: new Date(2020, 11, 10, 13, 12, 04)},
    { x: new Date(2020, 11, 10, 13, 12, 05)},
    { x: new Date(2020, 11, 10, 13, 12, 06)},
    { x: new Date(2020, 11, 10, 13, 12, 07)},
    { x: new Date(2020, 11, 10, 13, 12, 08)},
    { x: new Date(2020, 11, 10, 13, 12, 09)},
    { x: new Date(2020, 11, 10, 13, 12, 10)},
    { x: new Date(2020, 11, 10, 13, 12, 11)}
  ];

let labels7 = ['AltitudeQnh_meters'];
let data5 = [66.7,69.42,92.56,78.23,71.7,99.98,75.99,90.41,65.52,80.64,0,0,0];


let myChart5 = document.getElementById("myChart5").getContext('2d');

let chart5 = new Chart(myChart5, {
  type: 'line',
  data: {
    labels: labels6,
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
        display: false,
        //type: 'time',
      }]
    },
    legend: {
      postition: 'bottom',
      display: false
    }
  }

});

let labels10 = ['AirSpeed_knots'];
let data6 = [8.1,7,6.7,4.6,1.7,5.3,0.2,7.5,8.2,7.3,0,0,0];


let myChart6 = document.getElementById("myChart6").getContext('2d');

let chart6 = new Chart(myChart6, {
  type: 'line',
  data: {
    labels: labels6,
    datasets: [{
      label: 'AirSpeed (knots)',
      data: data6,
      fill: false,
      backgroundColor: 'red'
    }]
  },
  options: {
    scales: {
      xAxes: [{
          display: false,
          type: 'time',
      }],
      yAxes: [{
        scaleLabel: {
          display: true,
          labelString: 'AirSpeed (knots)'
        }
      }]
    },
    title: {
      text: "test",
      display: false
    },
    legend: {
      postition: 'left',
      display: false
    }
  }

});

let coldData = [35,35,35,35,35,35,35,35,35,35];
let hotData = [25,25,25,25,25,25,25,25,25,25];

var userDataSchema = new Schema({
TimeStampDate: String,              // Time GMT
AltitudeQnh_meters: Number,          // Pressure altitude meters
AirSpeed_knots: Number,              // GPS speed knots
VerticalSpeed_ms: Number,          // Vertical Airspeed meters per second
NormalAcceleration_g: Number,       // Normal acceleration g
MotorPower_volts: Number,           // Motor power voltage
OutsideTemp_celsius: Number,       // Temperature celsius

});

var myschema = new userDataSchema({
  TimeStampDate: "2020/12/26 14:22:01",
  AltitudeQnh_meters: 30,
  AirSpeed_knots: 14.5,
  VerticalSpeed_ms: 1.2,
  NormalAcceleration_g: 0.85,
  MotorPower_volts: 7.4,
  OutsideTemp_celsius: 13.5,
});

console.log(myschema);

var ctx = document.getElementById('tempChart').getContext('2d');

var chart = new Chart(ctx, {
  type: 'line',
  data: {
      labels: labels6,
      legend: {
         display: true
      },
      datasets: [{
          fill: false,
          data: coldData,
          label: 'Hot Temperature',
          backgroundColor: "#FF2D00",
          borderColor: "#FF2D00",
      },
      {
          fill: false,
          data: hotData,
          label: 'Cold Temperature',
          backgroundColor: "#0027FF",
          borderColor: "#0027FF",
      }]
  },
  options: {
    animation: false,
    responsive: true,
    scales: {
      xAxes: [{
          scaleLabel: {
            display: true,
            labelString: 'Time ( UTC )'
          },
          type: 'time',
          ticks: {
            maxRotation: 90,
            minRotation: 90
          }
      }],
      yAxes: [{
        scaleLabel: {
          display: true,
          labelString: 'Temperature ( Celcius )'
        },
      }]
    },
    legend: {
      display: false
    }
  }
});
