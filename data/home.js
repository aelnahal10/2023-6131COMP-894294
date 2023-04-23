import React, { useEffect, useState } from 'react';
import axios from 'axios';
import ReactDOM from 'react-dom';
import Home from './home';

ReactDOM.render(<Home />, document.getElementById('root'));

const result = await axios('http://192.168.112.216/readings');

function Readings() {
  const [temperature, setTemperature] = useState(0);
  const [humidity, setHumidity] = useState(0);

  useEffect(() => {
    const fetchData = async () => {
      const result = await axios('/readings');
      setTemperature(result.data.temperature);
      setHumidity(result.data.humidity);
    };
    
    fetchData();
  }, []);


  return (
    <div>
      <h2>Temperature: {temperature}</h2>
      <h2>Humidity: {humidity}</h2>
    </div>
  );
}

export default Readings;
