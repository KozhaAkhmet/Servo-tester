#include <Servo.h>               // Library for servos

#include <Adafruit_GFX.h>        // Library for Oled screen
#include <Adafruit_SSD1306.h>    


#define SCREEN_WIDTH 128 // Oled display width, in pixels
#define SCREEN_HEIGHT 32 // Oled display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo servo;        // Creating servo


int potPinRegulator = A0;  // First potentiometer
int potPinServo = A1;  // Seconr potentiometer
int servoPin =  9;
int buttonPin = 3;

int page = 0;         // For page number
bool pressed = false; // To prevent button sticking

void setup() {
  servo.attach(servoPin);     // Attaching servo to servo`s pin
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Enable Oled display
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);                        // Enable serial
}

void loop() {
  // Making a memory allocation and creating values in a pointer array.
  int *data = (int*)malloc(4*sizeof(int));
  
  // Reading raw values.
  data[0]=analogRead(potPinRegulator);  
  data[1]=analogRead(potPinServo);  

  // Converting raw values to angles. 
  data[2]=map(analogRead(potPinRegulator), 0, 685, 0, 180);   // Requere manually measuring 90 degree according to potentiometer value. In my case, It 685.
  data[3]=map(analogRead(potPinServo), 0, 1023, 0, 270);
 
  servo.write(data[3]);    // Wrtiting data to servo.

  //TODO control button connections.
  if(digitalRead(buttonPin) == 1 && !pressed){  // To list page once when pressed.
  page++;
  pressed = true;
  }
  else if(!digitalRead(buttonPin) && pressed)   //
  pressed = false;

  switch (page) {  // Displaying pages. This section can be modified by adding extra pages.
    case 0: 
      page1(data);
      break;
    case 1: 
      page2(data);
      break;
    default:
     page = 0;     // Reset page
  }

  Serial.print(digitalRead(buttonPin));
  Serial.print(" ");
  Serial.print(page);
  Serial.print("\n");
  

  
  free(data);
  delay(500);
}

void page1(int *data ){
  // First clearing display buffer.
  display.clearDisplay();
  
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  
  // Printing first coulomn.
  display.setCursor(0,0);
  display.print(F("Input Ang:"));
  display.print(data[2]);

  display.setCursor(0,8);
  display.print(F("Output Ang:"));
  display.print(data[3]);

  // 2nd coulomn.
  display.setCursor(0,16);
  display.print(F("Dif:"));
  display.print(data[3]-data[2]);  // Difference between pot angles.

  display.setCursor(0,24);             
  display.print(F("Per:"));
  display.print((5* (data[3]-data[2]))/9); // Mistake between angles in persent.
  display.print(F("%"));
  
  display.display();
}
void page2(int *data ){
  display.clearDisplay();

// Printing first coulomn.
  display.setCursor(0,0);
  display.print(F("Input Ang:"));
  display.print(data[2]);

  display.setCursor(0,8);
  display.print(F("Output Ang:"));
  display.print(data[3]);

// Printing Raw values
  display.setCursor(0,16);
  display.print(F("Inpt pot raw val:"));
  display.print(data[1]);

  display.setCursor(0,24);
  display.print(F("Srvo pot raw val:"));
  display.print(data[0]);

  display.display();
}



