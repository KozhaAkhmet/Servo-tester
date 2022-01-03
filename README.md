# Servo Tester V0
## About 
Many servos have factory defects where servo can`t make correct angles.
 
 This problem can be fixed with map() funtion. This function is used to change from range to another.

But to find correct numbers of that ranges we need to measure reel angles from servo. For this i will use potentiometer.

When you working on big projects where you use much more servos and also use precised movements it needs practical devices. Then I think to make Servo Tester where you can fastly and precisely correct servo angles. I made it for my next Quadruped V2 where I use 12 servos. 

All parts that needed is :
- Arduino Nano 
- OLED 128x32
- Servo 
- 2X 10k Potetiometer
- Button  
- 47uf condensator or more
- PCB Prototype Board
  
We need to find value of starting point on **first** potentiometer which measure servo angle to make it more perceive. We need to turn potentiometer a little were value is neer to 1. Otherwise it measure null side. 

 ## I marked starting point to help when i mount servo attacher...
![1](/Servo_tester/1.png)

Then turn that potentiometer to make 180 deg then note measured value. We use this when convert potentiometer value to degree.

We did not do it on **second** potentiometer.

Then tinkering..
#
## Embedding all parts to board...
![1](images/IMG_20220101_171009.jpg)
![1](images/IMG_20220101_173322.jpg)
![1](images/IMG_20220101_173332.jpg)
![1](images/IMG_20220101_182704.jpg)
![1](images/IMG_20220101_182711.jpg)
![1](images/IMG_20220101_215255.jpg)
## I add 47uf condensator to compensate consumptions...
![1](/Servo_tester/IMG_20220101_225646.jpg)
## Made little platform for servo...
![1](/Servo_tester/IMG_20220102_024539.jpg)
#
# Software
At this part i used this Libraries:
```c
    #include <stdlib.h>            //For Pointers
    #include <SPI.h>
    #include <Wire.h>
    #include <Servo.h>             //Library for servos
    #include <Adafruit_GFX.h>      //These are for OLED screen
    #include <Adafruit_SSD1306.h>  //
    #include <stdarg.h>
```

Making an memory allocation and creating values in pointer array.
```c
    int *data = (int*)malloc(4*sizeof(int));     
```
Value of first potentiometer..
```c
    data[0]=analogRead(pot_pin1); //
```
Value of second potentiometer..
```c
    data[1]=analogRead(pot_pin2); //Value of s
```
   Converting **measured** first potentiometer values to angles...
```c
    data[2]=map(analogRead(pot_pin1),0,669,0,180);
```
Converting  second potentiometer values to angles...
```c
    data[3]=map(analogRead(pot_pin2),0,1023,270,0);
```
#
##  void seriallogDetailes(int *data)
This function for printing values on Serial port.
```c
    serialPrintf("pot1: %4d\tpot2: %4d\tangle1: %3d\tangle2: %3d\n",data[0],data[1],data[2],data[3]);
```
Arduino not able to print with **Formatted string**.

To make it possible i added this from this [site](https://medium.com/@kslooi/print-formatted-data-in-arduino-serial-aaea9ca840e3).
```C
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
```

## void oledLogDetailes(int *data )
This function for printing values on OLED screen.
it needs do define this libraries. You can find that on Arduino Library Manager.
```c
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
```
Defining OLED size and reset pin
```C
    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 32 // OLED display height, in pixels

    #define OLED_RESET     4 
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
```
Enable it on setup
```c
    void setup(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }
```
```c
    display.clearDisplay();
  display.setTextSize(1);                //Prepairing for printing
  display.setTextColor(SSD1306_WHITE);   //
  
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
  display.setCursor(75,16);
  display.print(F("Dif:"));
  display.print(data[3]-data[2]);         //difference between pot angles

  display.setCursor(75,24);             
  display.print(F("Per:"));
  display.print((5* (data[3]-data[2]))/9); //mistake persentage between angles
  display.print(F("%"));
  
  display.display();   //This needed to enable changes on screen



