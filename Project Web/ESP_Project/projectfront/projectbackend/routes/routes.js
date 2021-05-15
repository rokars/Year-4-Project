var express = require('express');
var router = express.Router();

router.post('/postData', async (request, response) =>{
  const MongoClient = require('mongodb').MongoClient;
  const assert = require('assert');

  MongoClient.connect(process.env.DATABASE_ACCESS, function(err, client) {
    assert.strictEqual(null, err);

    const db = client.db("Y4_ESP32_Data");

    db.collection('collection_1').insertOne({
      date:new Date().toISOString().toString(),
      Magnetometer_X:request.body.Magnetometer_X,
      Magnetometer_Y:request.body.Magnetometer_Y,
      Magnetometer_Z:request.body.Magnetometer_Z,
      Gyroscope_X:request.body.Gyroscope_X,
      Gyroscope_Y:request.body.Gyroscope_Y,
      Gyroscope_Z:request.body.Gyroscope_Z,
      Accelerometer_X:request.body.Accelerometer_X,
      Accelerometer_Y:request.body.Accelerometer_Y,
      Accelerometer_Z:request.body.Accelerometer_Z,
      Temperature:request.body.Temperature,
      Pressure:request.body.Pressure,
      GPS_String:request.body.GPS_String
    }).then(successCallback, failureCallback);

    function successCallback() {
      response.status(201).send("Success")
      client.close();
    }
    
    function failureCallback(error) {
      response.status(500).send("ERROR")
    }

  });

});

router.get('/getDB', async (request, response) =>{

  const MongoClient = require('mongodb').MongoClient;
  const assert = require('assert');
  var dataArr = [];

  // Use connect method to connect to the Server
  MongoClient.connect(process.env.DATABASE_ACCESS, function(err, client) {
    assert.strictEqual(null, err);

  const db = client.db("Y4_ESP32_Data");
  var cursor = db.collection('collection_1').find({});

  function iterateOver(doc) {
    dataArr.push(doc)
  }

  function successCallback() {
    client.close();
    response.json(dataArr)
  }
  
  function failureCallback(error) {
    response.status(500).send("ERROR")
  }

  cursor.forEach(iterateOver).then(successCallback, failureCallback);

  });

});


module.exports = router