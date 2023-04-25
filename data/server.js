const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const app = express();
const fs = require('fs');

app.use(cors());
app.use(bodyParser.json());

let readings = {
  temperature: 0,
  humidity: 0
};

let ip = "192.168.112.216";

app.post('/readings', (req, res) => {
  const data = req.body;
  console.log("Temprature:" ,req.body.temperature)
  console.log("Humidty:" ,req.body.humidity)
  saveData(data.temperature, data.humidity);
  res.sendStatus(200);
});

app.listen(8081,ip, () => {
  console.log('Server started on port 8081');
});


function saveData(temperature, humidity) {
  const data = `Temperature: ${temperature} \nHumidity: ${humidity}\n`;
  fs.appendFile('data.txt', data, (err) => {
    if (err) {
      console.error(err);
      return;
    }
    console.log('Data saved to file');
  });
}

