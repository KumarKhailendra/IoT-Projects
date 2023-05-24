#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// make some custom characters:
byte cactus[8] = {
  0b00101,
  0b10101,
  0b10101,
  0b10101,
  0b10110,
  0b01100,
  0b00100,
  0b00100
};

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01001
};

byte dino[8] = {
  0b01110,
  0b10111,
  0b11110,
  0b11111,
  0b11000,
  0b11111,
  0b00110,
  0b10010
};

byte dinom[8] = {
  0b01110,
  0b10111,
  0b11110,
  0b11111,
  0b11000,
  0b11111,
  0b01001,
  0b00101
};

void setup() {
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);

  // create a new character
  lcd.createChar(0, cactus);
  // create a new character
  lcd.createChar(1, smiley);
  // create a new character
  lcd.createChar(2, frownie);
  // create a new character
  lcd.createChar(3, dino);
  // create a new character
  lcd.createChar(4, dinom);

  // set the cursor to the top left
  lcd.setCursor(0, 0);

  lcd.write(byte(0)); 
  lcd.write((byte)1);

}

void loop() {
  lcd.setCursor(4, 1);
  lcd.write(3);
  delay(250);
  lcd.setCursor(4, 1);
  lcd.write(4);
  delay(250);
}
