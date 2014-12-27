An arduino sketch to measure temperature with a Maxim DS18B20 Programmable Resolution 1-Wire Digital Thermometer connected to pin 2.

Makes a high resolution temperature measurement every 5 seconds and stores its 1 minute exponential moving average.

An Ethernet server on port 80 (ethernet shield required) provides this time-averaged temperature to clients.