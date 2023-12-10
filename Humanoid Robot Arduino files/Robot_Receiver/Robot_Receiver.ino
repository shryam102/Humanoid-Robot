
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
Servo left_hand;
Servo left_elbow;
Servo head;
Servo right_elbow;
Servo right_hand;
int left_handVal, left_elbowVal, headVal, right_elbowVal, right_handVal;
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1X;
  byte j1Y;
  byte j2X;
  byte j2Y;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData(); 
  left_hand.attach(14); 
  left_elbow.attach(15);
  head.attach(16);
  right_elbow.attach(17);
  right_hand.attach(18); 
}
void loop() {

  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone jas a throttle up, if we lose connection it can keep flying away if we dont reset the function
  }

  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }

  // Controlling servos
  left_handVal = map(data.j1Y, 127, 255, 180, 0); // Map the receiving value form 0 to 255 to 0 to 180(degrees), values used for controlling servos
  left_elbowVal = map(data.j1X, 0, 255, 0, 180);
  headVal = map(data.j2Y, 0, 255, 0, 180);
  right_elbowVal = map(data.j2X, 0, 255, 180, 0);
  right_handVal = map(data.j1Y, 0, 127, 180,0); 
  left_hand.write(left_handVal);
  left_elbow.write(left_elbowVal);
  head.write(headVal);
  right_elbow.write(right_elbowVal);
  right_hand.write(right_handVal);
  Serial.println(data.j1X);
}
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1X = 127;
  data.j1Y = 127;
  data.j2X = 127;
  data.j2Y = 127;
}
