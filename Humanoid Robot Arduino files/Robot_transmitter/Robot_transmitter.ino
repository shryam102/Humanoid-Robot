#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";


struct Data_Package {
  byte j1X;
  byte j1Y;
  byte j2X;
  byte j2Y;
};
Data_Package data; //Create a variable with the above structure


void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
}

void loop() {
  data.j1X = map(analogRead(A0), 0, 1023, 0, 255);
  data.j1Y = map(analogRead(A1), 0, 1023, 0, 255);
  data.j2X = map(analogRead(A2), 0, 1023, 0, 255);
  data.j2Y = map(analogRead(A3), 0, 1023, 0, 255);
  radio.write(&data, sizeof(Data_Package));

}
