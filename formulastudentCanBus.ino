#include <esp32_can.h> //https://github.com/collin80/esp32_can
//https://www.youtube.com/watch?v=Se2KCVyD7CM
//https://github.com/collin80/esp32_can/blob/master/examples/CANTestESP32_BuiltIn/CANTestESP32_BuiltIn.ino

void printFrame(CAN_FRAME *message)
{
  Serial.print(message->id, HEX); //header, what the info is
  if (message->extended) Serial.print(" X ");
  else Serial.print(" S ");   
  Serial.print(message->length, DEC); 
  for (int i = 0; i < message->length; i++) {
    Serial.print(message->data.byte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  CAN0.setCANPins(); //idk what to put here
  Serial.println("Initializing ...");
  CAN0.begin(500000);
  Serial.println("Ready ...!");
}


void loop() {
  CAN_FRAME can_message
  if(CAN0.read(can_message)){
      printFrame(&can_message));
  }
}
