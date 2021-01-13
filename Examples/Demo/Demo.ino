#include <max7219.h>

MAX7219 max7219(12, 11, 10);

void setup() {
  Serial.begin(9600);
  max7219.Begin();
}

void loop() {
  String temp;
  char temp2[8];
  int y;

  //DisplayText Demo
  max7219.DisplayText("95.67F", true); //Right justified
  delay(3000);
  max7219.Clear();
  max7219.DisplayText("95.67F", false); //Left justified
  delay(3000);
  max7219.Clear();

  //Counter with decimals
  //slow counter
  for (float x = 0; x < 1; x = x + 0.1) {
    temp = String(x);
    temp.toCharArray(temp2, temp.length());
    max7219.DisplayText(temp2, false); //0=left justify 1=right justify
    Serial.println(x);
    delay(500);
  }
  //fast counter
  for (float x = 0; x < 500; x++) {
    temp = String(x);
    temp.toCharArray(temp2, temp.length());
    max7219.DisplayText(temp2, false); //0=left justify 1=right justify
    Serial.println(x);
  }
  delay(500);

  //Display Char Demo
  max7219.Clear();
  max7219.DisplayChar(7, 'H', false); //Position 7 is on the left of the display
  delay(500);
  max7219.DisplayChar(6, 'E', false);
  delay(500);
  max7219.DisplayChar(5, 'L', false);
  delay(500);
  max7219.DisplayChar(4, 'L', false);
  delay(500);
  max7219.DisplayChar(3, 'O', false);
  delay(500);
  max7219.DisplayChar(2, '1', false);
  delay(500);
  max7219.DisplayChar(1, '2', false);
  delay(500);
  max7219.DisplayChar(0, '3', false);
  delay(500);
  max7219.Clear();
  //Count front the right
  for (uint8_t x = 0; x < 8; x++) {
    max7219.DisplayChar(x, 48 + x, false); //48 is ASCII value for 0
    delay(500);
  }
  max7219.Clear();
  delay(500);
  //Count from the left
  for (uint8_t x = 7; x >= 0; x--) {
    max7219.DisplayChar(x, 48 + (7 - x), false); //48 is ASCII value for 0
    delay(500);
  }
  max7219.Clear();
  //Count from the right
  for (uint8_t x = 0; x < 8; x++) {
    max7219.DisplayChar(x, 48 + x, false); //48 is ASCII value for 0
    delay(500);
    max7219.Clear();
  }
  delay(500);
  //Count front the left
  for (uint8_t x = 7; x >= 0; x--) {
    max7219.DisplayChar(x, 48 + (7 - x), false); //48 is ASCII value for 0
    delay(500);
    max7219.Clear();
  }
}
