let labels1 = ['YES', 'YES BUT IN GREEN'];
let data1 = [69, 31];
let colors1 = ['blue', 'green'];


let myChart1 = document.getElementById("myChart1").getContext('2d');

let chart1 = new Chart(myChart1, {
  type: 'doughnut',
  data:
  {
    labels: labels1,
    datasets:
    [{
      data: data1,
      backgroundColor: colors1
    }]
  },
  options:
  {
    title:
    {
      text: "Do you like doughnuts?",
      display: true
    }
  }

});


let labels2 = ['American Airlines Group', 'Ryanair', 'China Southern Airlines', 'Lufthansa Group'];
let data2 = [199.6, 130.3, 126.3, 130];
let colors2 = ['blue', 'green', 'red', 'yellow'];


let myChart2 = document.getElementById("myChart2").getContext('2d');

let chart2 = new Chart(myChart2, {
  type: 'bar',
  data:
  {
    labels: labels2,
    datasets:
    [{
      data: data2,
      backgroundColor: colors2
    }]
  },
  options:
  {
    title:
    {
      text: "Number of passengers carried in 2017 (millions)",
      display: true
    },
    legend:
    {
      display: false
    }
  }

});


let labels5 = ['TimeStampDate'];
let labels6 = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30];
let labels7 = ['AltitudeQnh_meters'];
let data5 = [66.7,69.42,92.56,78.23,71.7,99.98,75.99,90.41,65.52,80.64,84.05,73.79,55.39,52.26,93.36,84.28,71.45,83.29,76.59,60.38,83.85,97.16,54.18,59.21,61.59,68.88,74.54,54.81,70.85,88.18];


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
        }
      }]
    },
    title: {
      text: "test",
      display: true
    },
    legend: {
      display: true
    }
  }

});

let labels8 = ['TimeStampDate'];
let labels9 = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30];
let labels10 = ['AirSpeed_knots'];
let data6 = [8.1,7,6.7,4.6,1.7,5.3,0.2,7.5,8.2,7.3,6.4,6.6,4.8,4.1,5.9,7.7,5.6,6,5.5,7.6,8.1,3.2,3.6,1.3,1.2,5,1.1,9.5,9.3 ];


let myChart6 = document.getElementById("myChart6").getContext('2d');

let chart6 = new Chart(myChart6, {
  type: 'line',
  data:
  {
    labels: labels9,
    datasets: [{
      data: data6,
      fill: false,
      backgroundColor: 'red'
    }]
  },
  options:
  {
    title:
    {
      text: "test",
      display: true
    },
    legend:
    {
      display: true
    }
  }

});
