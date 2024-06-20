#include <Encoder.h>
#include <Adafruit_GFX.h>  // Library for Oled screen
#include <Adafruit_SSD1306.h>
#include <Servo.h>  // Library for servos

#define SCREEN_WIDTH 128  // Oled display width, in pixels
#define SCREEN_HEIGHT 32  // Oled display height, in pixels

#define OLED_RESET 4  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SERIAL_DEBUG true

#define POT_PIN A1
#define SERVO_PIN 9
#define BUTTON_PIN 3
#define OUT_PIN A0
#define ENCODER_STEPS 20
#define STEP_IN_MM 0.429166667 // The original length of the dingle step is 1.2875 mm (20 total holes from encoder divided to 25.75 the diameter of the encoder),
                               // then by 1:4 gear ratio ( dividing the value by three) we get 0.429166667 value.
#define STEP_IN_DEGREE 4.5// Similarly we can measure the degree. ((360 / 20) /4)
Servo servo;  // Creating servo class
Encoder encoder(OUT_PIN, ENCODER_STEPS);

int page = 0;         // For page number
bool pressed = false; // To prevent button sticking
int pot_value;
byte button_value;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Enable Oled display
  pinMode(BUTTON_PIN, INPUT);
  servo.attach(SERVO_PIN);  // Attaching servo to servo`s pin
  encoder.Setup();
  encoder.setDir(1);

  if(SERIAL_DEBUG)
    Serial.begin(9600);  // Enable serial

  delay(10);
}

int sum = 0;
int lightValue = 0;

void loop() {
  for (int i = 0; i < 10; i++) {
    lightValue = analogRead(POT_PIN);
    sum += lightValue;
  }
  pot_value = sum / 10;
  sum = 0;
  button_value = digitalRead(BUTTON_PIN);
  encoder.lapCount();

  if(button_value == 1 && !pressed){  // To list page once when pressed.
    page++;
    pressed = true;
  }
  else if(!button_value && pressed)   
    pressed = false;

  switch (page) {  // Displaying pages. This section can be modified by adding extra pages.
    case 0: 
      // servo_calibration();
      start_calibration();
      break;
    // case 1: 
    //   page2();
    //   break;
    default:
     page = 0;     // Reset the page
  }

  if(SERIAL_DEBUG)
    serial_print();
}

void servo_calibration() {
  // if(pot_val == 0)
  // First clearing display buffer.
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Printing first coulomn.
  display.setCursor(0, 0);
  display.print(F("Lap Count: "));
  display.print(encoder.getSteps());

  display.setCursor(0, 8);
  display.print(F("Steps in mm: "));
  display.print(STEP_IN_MM * encoder.getSteps());
  
  display.setCursor(0, 16);
  display.print(F("Degree: "));
  display.print(STEP_IN_DEGREE * encoder.getSteps());

  display.setCursor(0, 24);
  display.print(F("Output Ang:"));
  // display.print(data[3]);

  // 2nd coulomn.
  
  servo.write(map(pot_value, 0, 1023, 200, 0));

  display.display();
}

void page2() {
  display.clearDisplay();

  // Printing first coulomn.
  display.setCursor(0, 0);

  display.display();
}

int accum = 500;
float temp = 0;
int min_value = 0;

void start_calibration(){
  if(accum < 700 || accum > 2300)
    accum += 1;
  else
    accum += 10;

  if(accum > 2500){
    accum = 400;
    encoder.reset();
  }
  servo.writeMicroseconds(accum);
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Printing first coulomn.
  display.setCursor(0, 0);
  display.print(F("Microseconds: "));
  display.print(accum);

  display.setCursor(0, 8);
  display.print(F("Degree: "));
  display.print(STEP_IN_DEGREE * encoder.getSteps());

  display.setCursor(0, 16);
  display.print(F("Min : "));
  if(encoder.getSteps() < 1)
    min_value = accum;
  display.print(min_value);
  
  display.setCursor(0, 24);
  display.print(F("Max : "));
  // if(encoder.getSteps() == 1)
  //   display.print(STEP_IN_MM * encoder.getSteps());
  
  display.display();
  delay(50);
}

void serial_print(){
  Serial.print("Button value: ");
  Serial.print(button_value);
  Serial.print(" Pot Value: ");
  Serial.print(pot_value);
  Serial.print(" Page: ");
  Serial.print(page);
  Serial.print("\n");
}
