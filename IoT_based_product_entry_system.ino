
// IoT based Product Entry System

#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>                                     // This Keypad library is installed from github version of Keypad Lobrary


const char* ssid = "*********";                         // Enter your WiFi Credentials 
const char* password = "*********";       
ESP8266WebServer server(80);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {10, 9, D3, D4};                  // 9 is SDD2 & 10 is SDD3
byte colPins[COLS] = {D5, D6, D7, D8};                 // Make sure that your pins are connected as  mentioned above orelse change acc to your convinient 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
const int noOfProducts = 10;                                   // customize the noOfProducts into your required no of products
int prodDetails[noOfProducts] = {0,0,0,0,0,0,0,0,0,0};   // make sure that the same no of zeros as noOfProducts                
int quanInt;   
int prodNumInt;

Adafruit_SSD1306 display(128, 32, &Wire);  

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid,password);
  Serial.print("Connecting to..");
  Serial.println(ssid);
  Serial.println("");  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
  Serial.println("Subnet NetMask");
  Serial.println(WiFi.subnetMask());
  Serial.println("Gateway");
  Serial.println(WiFi.gatewayIP());  
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.println("Display begins");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop()
{ 
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Press any Key to continue..");
  Serial.print("Press any Key to continue..");
  display.display();
  char key = customKeypad.getKey();
  if(key != NO_KEY)
  {
    uplodation();
  }
  
  server.send(200, "text/html", Send2Webpage(prodNumInt,quanInt,prodDetails));
  server.handleClient();
}

void handle_OnConnect()
{
  uplodation();
}

void handle_NotFound()
{
  server.send(404, "text/plain", " Not Found");
}


void uplodation()
{
  display.clearDisplay();
  employeeNumberUpload();
  productNumberUpload();
}

void employeeNumberUpload()
{
  display.setCursor(0,0);
  display.println("Enter Employee Number");
  Serial.println("Enter Employee Number");
  display.display();

  char empNum[4] = {};                           // getting employee number as a character array  
  empNum[0] =  customKeypad.waitForKey();  
  Serial.print(empNum[0]);
  display.print(empNum[0]);
  display.display();
  empNum[1] =  customKeypad.waitForKey();
  Serial.print(empNum[1]);
  display.print(empNum[1]);
  display.display();
  empNum[2] =  customKeypad.waitForKey();
  Serial.print(empNum[2]);
  display.print(empNum[2]);
  display.display();
  empNum[3] =  customKeypad.waitForKey();
  Serial.print(empNum[3]);
  display.print(empNum[3]);
  display.display();
  
  delay(3000);  
  display.clearDisplay();
  
  int empNumInt = atoi(empNum);                 // converting the character array into integer , because integer is compacible for calculations
  Serial.println("");
  Serial.println(empNumInt);                    // displaying after atoi
  display.println(empNumInt);   
  Serial.println(empNum); 
  //display.print(empNum);                      // displaying as array
  String empNum1 = String(empNum);
  Serial.println(empNum1); 
  //display.print(empNum1);                     //  displaying as string
  empNum1.toInt();
  Serial.println(empNum1); 
  //display.print(empNum1);                     // displaying as integer
   
  display.display();
  delay(2000);  
  display.clearDisplay();
}

void productNumberUpload()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Enter Product Number");
  Serial.println("Enter Product Number");
  display.display();
  
  char prodNum[2] = {};
  prodNum[0] =  customKeypad.waitForKey();      
  Serial.print(prodNum[0]);
  display.print(prodNum[0]);
  display.display();
  prodNum[1] =  customKeypad.waitForKey();
  Serial.print(prodNum[1]);
  display.print(prodNum[1]);
  display.display();

  delay(3000);  
  display.clearDisplay();
  prodNumInt = atoi(prodNum);
  Serial.println("");
  Serial.println(prodNumInt);
  display.print(prodNumInt);
  display.display();
  delay(2000);  
  display.clearDisplay();

  quantityUpload(prodNumInt);
}

void quantityUpload(int prodNumInt)
{
  display.setCursor(0,0);
  display.println("Enter Quantity");
  Serial.println("Enter Quantity");
  display.display();

  char quan[2] = {};
  quan[0] =  customKeypad.waitForKey();      
  Serial.print(quan[0]);
  display.print(quan[0]);
  display.display();
  quan[1] =  customKeypad.waitForKey();      
  Serial.print(quan[1]);
  display.print(quan[1]);
  display.display();
  
  delay(3000);
  display.clearDisplay();

  quanInt = atoi(quan);
  Serial.println("");
  Serial.println(quanInt);
  display.print(quanInt);
  display.display();
  delay(2000);  
  display.clearDisplay();

  prodDetails[prodNumInt-1]=prodDetails[prodNumInt-1]+quanInt;
  server.send(200, "text/html", Send2Webpage(prodNumInt,quanInt,prodDetails));  // sending the collected details to webpage
  quanInt = 0;  
}

String Send2Webpage(int prodNumInt,int quanInt,int prodDetails[noOfProducts])
{
 String ptr ="<!DOCTYPE html> <html> \n";
 ptr +="<head> <title> Product Details Page  </title> </head> \n";  // <style> table, th, td { border: 1 px solid black} </style> -- add this to head tag if you need border for the table
 ptr +="<body>";
 ptr +="<p> Current Sold Product ID is   ";
 ptr +=(int)prodNumInt;
 ptr +="</p>";
 ptr +="<p> and its quantity is   ";
 ptr +=(int)quanInt;
 ptr +="</p>";
 ptr +="<b> <u> Product Details </u> </b> \n";
 ptr +="</br>";
 ptr +="<table>";
 ptr +="<tr>";
 ptr +="<th>Product ID </th>";
 ptr +="<th>Quantity </th>";
 ptr +="</tr>";
 for(int i=0;i<noOfProducts;i++)
 {
  ptr +="<tr>";
  ptr +="<td>";
  ptr +=(int)i+1;
  ptr +="</td>";
  ptr +="<td>";
  ptr +=(int) prodDetails[i];
  ptr +="</td>";
  ptr +="</tr>";
 }
 ptr +="</table>";
 ptr +=" </body> </html>"; 
 return ptr;
}
