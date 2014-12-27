#include <SPI.h>
#include <Ethernet.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192,168,1,5);

int TEMPERATURE_PRECISION = 12;
int measurement_interval = 5000; // milliseconds
float k = measurement_interval/60000.0; // 1 minute exponential smoothing constant
float T;
unsigned int time_of_last_measurement; // milliseconds

DeviceAddress device_address;
EthernetServer server(80);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {   
  Ethernet.begin(mac, ip);
  server.begin();
  sensors.begin();
  delay(1000);
  sensors.getAddress(device_address, 0);
  sensors.setResolution(device_address, TEMPERATURE_PRECISION);
  delay(1000);
  sensors.requestTemperatures();
  delay(measurement_interval);
  T = sensors.getTempC(device_address);
  sensors.requestTemperatures();
  time_of_last_measurement = millis();
}

void loop(){
  EthernetClient client = server.available();
  if (client) {
    client.println(T);
    delay(10);
    client.stop();
  }
  unsigned int time_since_last_measurement = millis() - time_of_last_measurement; // modular arithmetic, always > 0
  if (time_since_last_measurement > measurement_interval){
    time_of_last_measurement = millis();
    float Tnew = sensors.getTempC(device_address);
    T = k*Tnew + (1-k)*T;
    sensors.requestTemperatures();
  }
}
