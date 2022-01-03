#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// C like printf();
#include <stdarg.h>
#define SERIAL_PRINTF_MAX_BUFF      256
void serialPrintf(const char *fmt, ...);void serialPrintf(const char *fmt, ...) {
  /* Buffer for storing the formatted data */
  char buff[SERIAL_PRINTF_MAX_BUFF];  /* pointer to the variable arguments list */
  va_list pargs;  /* Initialise pargs to point to the first optional argument */
  va_start(pargs, fmt);  /* create the formatted data and store in buff */
  vsnprintf(buff, SERIAL_PRINTF_MAX_BUFF, fmt, pargs);
  va_end(pargs);  Serial.print(buff);
}
// end of C like printf();

Servo servo;  
int pot_pin1 = A0;  // analog pin used to connect the potentiometer
int pot_pin2 = A1;  // analog pin used to connect the potentiometer which for measure angle

int input_micro=0;

void setup() {
  Serial.begin(9600); 
  servo.attach(9);     // attaches the servo on pin 9 to the servo object
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
}

void loop() {
  int *data = (int*)malloc(4*sizeof(int));
  data[0]=analogRead(pot_pin1);
  data[1]=analogRead(pot_pin2);
  data[2]=map(analogRead(pot_pin1),0,669,0,180);
  data[3]=map(analogRead(pot_pin2),0,1023,270,0);
/*
  int *val1=(int*)malloc(sizeof(int));
  int *val2=(int*)malloc(sizeof(int));
  *val1=analogRead(pot_pin1);
  *val2=analogRead(pot_pin2);
  
  input_angles1= map(analogRead(pot_pin1),0,751,0,180);
  input_angles2= map(analogRead(pot_pin2),0,751,0,180);
 */
  input_micro = analogRead(A1);
  
  servo.write(data[3]);
  //myservo.writeMicroseconds(map(inputmicro,0,1023,0,3000));

  serialLogDetails(data);
  oledLogDetailes(data);
  free(data);
  delay(100);
}

void oledLogDetailes(int *data ){
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0,0);
  display.print(F("Pot1:"));
  display.print(data[0]);
  
  display.setCursor(0,8);
  display.print(F("Pot2:"));
  display.print(data[1]);
  
  display.setCursor(0,16);
  display.print(F("Pot1 Ang:"));
  display.print(data[2]);
  
  display.setCursor(0,24);
  display.print(F("Pot2 Ang:"));
  display.print(data[3]);
  //2nd coulomb
  /*display.setCursor(75,0);
  display.print(F("Dif:"));
  display.print(data[1]-data[0]);         //difference between pot values

  display.setCursor(75,8);             
  display.print(F("Per:"));
  display.print(int abs((data[1]-data[0])/10.23)); //mistake persentage between values
  display.print(F("%"));*/

  display.setCursor(75,16);
  display.print(F("Dif:"));
  display.print(data[3]-data[2]);         //difference between pot angles

  display.setCursor(75,24);             
  display.print(F("Per:"));
  display.print((5* (data[3]-data[2]))/9); //mistake persentage between angles
  display.print(F("%"));
  
  display.display();
}

void serialLogDetails(int* data){
  serialPrintf("pot1: %4d\tpot2: %4d\tangle1: %3d\tangle2: %3d\n",data[0],data[1],data[2],data[3]);
}
