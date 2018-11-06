
//including libraries I think you have to only download SoftwareSerial library there others are installed by default but compile, the compiler will give error and you'll see which one to download
#include <Keypad.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

/*
PORTABLE SIREN
Arduino-Producing a wailing sound as a loud warning signal
LED Flsher for Visual Alert
source: https://www.electroschematics.com/9548/arduino-siren-sound-alarm/
*/

#define LEDFLASHER 48
#define ALARMSOUNDER 26 
#define ALARMSWITCH 8
int i = 0; 
int val = LOW; 
int pre_val = LOW; 
int state = 0;

//making an 'instance' of GPRS when ever we use serial monitor I will call it
SoftwareSerial GPRS(51,49);
//these are pins of LCD display rs, en, d4, d5, d6 and d7
LiquidCrystal lcd(40,38,36,34,32,30);


//This is for a 4x4 Keypad 
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {31, 33, 35, 37}; //Row pinouts of the keypad
byte colPins[COLS] = {39, 41, 43, 45}; //Column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//I use this to store SMS number
char Data[15];
byte data_count = 1, master_count = 0;
int dum = 0;

//I use this to store phone call number
char Data1[15];
byte data_count1 = 0;
int dum1 = 0;

int b = 0;
String textMessage;

dht DHT;
#define DHT11_PIN 28
                                                        //these are my random numbers still have to include sensors to get real values 
                                                        float voltage = 97.34;
                                                        float temperature = 25.34;
                                                        int humadity = 75; 
                                                        char degree = 223; //symbol for a degree celcius

                                                        //GPRS.print("AT+HTTPPARA=\"URL\",\"mmbhobho.000webhostapp.com/write_data.php?voltage=11.29&current=1.7&phase=0.91&power=16.5&temperature=24&humadity=74\"");
                                                        float current=1.7;
                                                        float phase=0.91;
                                                        float power=16.5;

 
                                                        //I use to count how I display menu
                                                        int n = 0;
                                                        int aa = 0;

//initilising status of the relays 
  char relay1[10] = "OPEN";
  char relay2[10] = "OPEN";
  char relay3[10] = "OPEN";
  char relay4[10] = "OPEN";
  char relay5[10] = "OPEN";


void setup() 
{ 
  delay(5000);
  
  GPRS.begin(9600);
  Serial.begin(9600);
 
  lcd.begin(16, 2);

  //This is where relays will ve connected
  pinMode(52,OUTPUT);
  pinMode(50,OUTPUT);
  pinMode(48,OUTPUT);
  pinMode(46,OUTPUT);
  pinMode(44,OUTPUT);


      pinMode(LEDFLASHER, OUTPUT); 
    pinMode(ALARMSOUNDER, OUTPUT); 
    pinMode(ALARMSWITCH, INPUT);


  lcd.setCursor(4,0);
  lcd.print("GSM based");
  lcd.setCursor(3,1);
  lcd.print("Substation");
  delay(3000);
  lcd.clear();
  lcd.print("Monitoring and");
  lcd.setCursor(0,1);
  lcd.print("Control System");
  delay(3000);
          
  showMenu();
  controlWeb();


}

void loop() 
{
  int chk = DHT.read11(DHT11_PIN);
  temperature = DHT.temperature;
  humadity = DHT.humidity;
 //Remember the global variable n above, it is in use now
  
                if(n==6)
                  {
                    showMenu();
                    
                    //requestweb();
                    n = 0;
                  }
                    n++;
                    recieveSMS();
                    showAll();
                    requestweb();
  
}

//This Function shows the Menu A, B, C and D. How to use the system
void showMenu()
{
  char key = keypad.getKey();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A: substation");
  lcd.setCursor(0, 1);
  lcd.print("Web Application");
    
  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts 
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("B: Control");
  lcd.setCursor(0, 1);
  lcd.print("Fans and Alarms");

  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                  // included wait so I could listen to the keypad I failed to use interrupts 
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("C: Receive");
  lcd.setCursor(0, 1);
  lcd.print("SMSnotifications");

  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts 
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D: Receive or");
  lcd.setCursor(0, 1);
  lcd.print("make phone call");


  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                  // included wait so I could listen to the keypad I failed to use interrupts 
    }
  }

}

//This Function shows substation paramters voltage, current power, power factor and temperature and humidity
void showAll()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("temperature");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" ");
  lcd.print(degree);
  lcd.print("C");
  
  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
    }
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("humidity");
  lcd.setCursor(0, 1);
  lcd.print(humadity);
  lcd.print(" %");
  
  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Battery Voltage");
  lcd.setCursor(0, 1);
  lcd.print(voltage);
  lcd.print(" %");
  
  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
    }
  }
  
} 

//Wait listen to the keypad while I am showing things on the LCD. According to the Menu if key is pressed it does what the Menu says
void wait()
{ 
  char key = keypad.getKey();
  
            if (key)
            {
              lcd.clear();
              lcd.print(key);
              delay(500);
              switch(key)
              {
                case 'A' :
                menuA();
                break;
                case 'B' :
                menuB();
                break;
                case 'C' :
                menuC();
                break;
                case 'D' :
                menuD();
                break;
                default:
                break;
              }
            }
          
        
}

void menuA()
{
  lcd.clear();
  lcd.print("visit");
  lcd.setCursor(0, 1);
  lcd.print("mmbhobho.github.io");
  delay(2000);
    for (int positionCounter = 0; positionCounter < 3; positionCounter++) 
    {
    lcd.scrollDisplayLeft();
    delay(150);
    }
    delay(2000);
  
  for(int i=0; i<1; i++)
  {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Features");
  lcd.setCursor(0, 1);
  lcd.print("Fans and Alarms");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Features");
  lcd.setCursor(4, 1);
  lcd.print("Graphs");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Features");
  lcd.setCursor(2, 1);
  lcd.print("Live videos");
  delay(1000); 
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Features");
  lcd.setCursor(0, 1);
  lcd.print("Send SMSmessages");
  delay(1000); 
  }
 recieveSMS();
}
void menuB()
{ if(b<1)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Control keys");
    lcd.setCursor(0, 1);
    lcd.print("use keypad");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan-1: 1 ");
    lcd.setCursor(0, 1);
    lcd.print("Fan-1: 2 ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarm-1: 3 ");
    lcd.setCursor(0, 1);
    lcd.print("Alarm-2: 4 "); 
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OFF : 5 ");
    delay(1000);
    b++;
}
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter key");   
  for(int i = 0; i <500; i++)
  {
    for(int a = 0; a <500; a++)
    {
        relays();              //This function listens to the keypad and toggles a relay if number[1-5] is pressed
    }
  } 
  controlWeb();

} 

void menuC()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Receive SMS");
    lcd.setCursor(0, 1);
    lcd.print("messages");
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("notifying with");
    lcd.setCursor(0, 1);
    lcd.print("substation parameters");
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("input cellno:");
    lcd.setCursor(0, 1);
    lcd.print("# to enter");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("input cellno:");  
    dum = smsNumber();    // this was a dum way of calling smsNumber()
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("sending sms to:");
    lcd.setCursor(0, 1);
    lcd.print(Data);
    sendSMS();
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
 
}

void menuD()
{
    lcd.clear();
    lcd.print("input cellno:");
    lcd.setCursor(0, 1);
    lcd.print("C to delete"); 
    delay(1000);
    lcd.clear();
    lcd.print("input cellno:");
    lcd.setCursor(0, 1);
    lcd.print("D to enter");
    delay(1000);
    lcd.clear();
    lcd.print("input cellno:");  
    dum1 = callNumber1();       //this too
    lcd.clear();
    lcd.print("calling:");
    lcd.setCursor(0, 1);
    lcd.print(Data1);
    call();
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts
        wait();                   // included wait so I could listen to the keypad I failed to use interrupts

}

// stores the number on the global variable Data[]
int smsNumber()
{

   char key = keypad.getKey();
   data_count = 0;

   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':

         //Data[0] = '+'; 
         Data[data_count] = key; // store char into data array
         lcd.setCursor(data_count, 1); // move cursor to show each new char
         lcd.print(Data[data_count]); // print char at said cursor
         data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered     
             
            break;

         case '*':
            data_count = data_count - 1;
            Data[data_count] = key;
            lcd.setCursor(data_count, 1);
            lcd.print(Data[data_count]);
            break;
      }

      key = keypad.getKey();
   }

   return 1;
}

//stores the number to global variable Data2[]
int callNumber1()
{

   char key = keypad.getKey();
   data_count1 = 0;

   while(key != 'D')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4': case '*':
         case '5': case '6': case '7': case '8': case '9': case '#': 

         //Data1[0] = '+'; for callback
         Data1[data_count1] = key; // store char into data array
         lcd.setCursor(data_count1, 1); // move cursor to show each new char
         lcd.print(Data1[data_count1]); // print char at said cursor
         data_count1++; // increment data array by 1 to store new char, also keep track of the number of chars entered     
             
            break;

         case 'C':
            data_count1 = data_count1 - 1;
            Data1[data_count1] = key;
            lcd.setCursor(data_count1, 1);
            lcd.print(Data1[data_count1]);
            break;
         case 'B':
         Data1[data_count1] = ';';
         lcd.print(Data1[data_count1]); // print char at said cursor
         break;
      }

      key = keypad.getKey();
   }

   return 1;
}


//listens to the keypad
void relays()
{ 
  char key = keypad.getKey();
  
            if (key)
            {
              lcd.clear();
              lcd.print(key);
              delay(500);
              switch(key)
              {
                case '1' :
                Relay1();
                lcd.clear();
                lcd.print("Fan-1 : ");
                lcd.print(relay1);
                lcd.setCursor(0, 1);
                lcd.print("switching");
                delay(300);
                
                break;
                case '2' :
                Relay2();
                lcd.clear();
                lcd.print("Fan-2 : ");
                lcd.print(relay2);
                lcd.setCursor(0, 1);
                lcd.print("switching");
                delay(300);
                
                break;
                case '3' :
                Relay3();
                lcd.clear();
                lcd.print("Alarm-1 : ");
                lcd.print(relay3);
                lcd.setCursor(0, 1);
                lcd.print("switching");
                delay(300);
                
                break;
                case '4' :
                Relay4();
                lcd.clear();
                lcd.print("Alarm-2 : ");
                lcd.print(relay4);
                lcd.setCursor(0, 1);
                lcd.print("switching");
                delay(300);
                
                break;
                case '5' :
                lcd.clear();
                lcd.print("Power : ");
                lcd.print(relay5);
                lcd.setCursor(0, 1);
                lcd.print("switching");
                delay(300);
                Relay5();
                break;
                default:
                break;
              }
            }
        
}

void Relay1()
{ 
  if (digitalRead(52) == LOW)
  {
  assign(relay1);                    //this is the important function I think, should use it when I retrieve content from the web or I can call Relay-num-() functions
  digitalWrite(52, HIGH);
  }
  else
  {
  lowsign(relay1);
  digitalWrite(52, LOW);
  }
        
}
void Relay2()
{
  if (digitalRead(50) == LOW)
  {
  assign(relay2);
  digitalWrite(50, HIGH);
  }
  else
  {
  lowsign(relay2);
  digitalWrite(50, LOW);
  }
}
void Relay3()
{
  if (digitalRead(48) == LOW)
  {
  assign(relay3);
  digitalWrite(48, HIGH);
  
  alarm();
  
  }
  else
  {
  lowsign(relay3);
  digitalWrite(48, LOW);
  }
}
void Relay4()
{
  if (digitalRead(46) == LOW)
  {
  assign(relay4);
  digitalWrite(46, HIGH);
  police();
  }
  else
  {
  lowsign(relay4);
  digitalWrite(46, LOW);
  }
}
void Relay5()
{
  if (digitalRead(44) == LOW)
  {
  assign(relay5);
  digitalWrite(44, HIGH);
  }
  else
  {
  lowsign(relay5);
  digitalWrite(44, LOW);
  }
}


//This is the fuction that toggles text 
void assign(char* arr)
{
  strcpy(arr, "CLOSED");
}  

void lowsign(char* arr)
{
  strcpy(arr, "OPEN");
}  

void requestweb()
{
  //Initialising GSM module to connect to the internet 
  GPRS.println("AT");
  delay(500);
  showdata();
  
  GPRS.println("AT+HTTPINIT");
  delay(500);
  showdata();
  
  GPRS.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(500);
  showdata();

  GPRS.println("AT+SAPBR=1,1");
  delay(500);
  showdata();
 
  GPRS.println("AT+HTTPPARA=\"URL\",\"mmbhobho.000webhostapp.com/get_data.php\"");
  delay(500);
  showdata();
  

  GPRS.println("AT+HTTPACTION=0");
  showAll();
  showdata();

  GPRS.println("AT+HTTPREAD");
  showAll();
  changeLed();
  showdata();
  
                                                                                                                                //GPRS.print("AT+HTTPPARA=\"URL\",\"mmbhobho.000webhostapp.com/write_data.php?voltage=11.29&current=1.7&phase=0.91&power=16.5&temperature=24&humadity=74\"");
  GPRS.print("AT+HTTPPARA=\"URL\",\"mmbhobho.000webhostapp.com/write_data.php?");
  GPRS.print("voltage=");GPRS.print(voltage);GPRS.print("&current=");GPRS.print(current);GPRS.print("&phase=");GPRS.print(phase);GPRS.print("&power=");GPRS.print(power);GPRS.print("&temperature=");GPRS.print(temperature);GPRS.print("&humadity=");GPRS.print(humadity);
  GPRS.println("\"");
  showAll();
  delay(7000);
  showdata();

  GPRS.println("AT+HTTPACTION=0");
  showAll();
  delay(2000);
  showdata();  
                                                                                     //https://mmbhobho.000webhostapp.com/write_data2.php?relay1=OPEN&relay2=OPEN&relay3=OPEN&relay4=OPEN&relay5=OPEN
}

void controlWeb()
{
  //Initialising GSM module to connect to the internet 
  GPRS.println("AT");
  delay(500);
  showdata();
  
  GPRS.println("AT+HTTPINIT");
  delay(500);
  showdata();
  
  GPRS.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(500);
  showdata();

  GPRS.println("AT+SAPBR=1,1");
  delay(500);
  showdata();
                                                                                     //https://mmbhobho.000webhostapp.com/write_data2.php?relay1=OPEN&relay2=OPEN&relay3=OPEN&relay4=OPEN&relay5=OPEN
  GPRS.print("AT+HTTPPARA=\"URL\",\"mmbhobho.000webhostapp.com/write_data2.php?");
  GPRS.print("relay1=");GPRS.print(relay1);GPRS.print("&relay2=");GPRS.print(relay2);GPRS.print("&relay3=");GPRS.print(relay3);GPRS.print("&relay4=");GPRS.print(relay4);GPRS.print("&relay5=");GPRS.print(relay5);
  GPRS.println("\"");
  showAll();
  delay(7000);
  showdata();

  GPRS.println("AT+HTTPACTION=0");
  showAll();
  delay(2000);
  showdata(); 
}


void showdata()
{
  while(GPRS.available()!=0)
    Serial.write(char (GPRS.read()));
}

void changeLed()
{
 String content = "";
// String RedState = content.substring();
 while(GPRS.available()!=0)
 {  
    //Serial.write(SIM900.read());
   // content = Serial.readString();
    content = content + String(char (GPRS.read()));
 }
    Serial.println(content.substring(33,38));

    
 if(content.substring(33,34)== "0")
 {
   digitalWrite(52, HIGH);
   assign(relay1);
 }
 
 else if(content.substring(33,34)== "1")
 {
   digitalWrite(52, LOW);
   lowsign(relay1);
 }
 
 if(content.substring(34,35)== "0")
 {
   digitalWrite(50, HIGH);
   assign(relay2);
 }
 else if (content.substring(34,35)== "1")
 {
   digitalWrite(50, LOW);
   lowsign(relay2);
 }
 
 if(content.substring(35,36)== "0")
 {
   digitalWrite(48, HIGH);
   alarm();
   assign(relay3);
 }
 else if (content.substring(35,36)== "1")
 {
   digitalWrite(48, LOW);
   lowsign(relay3);
 }

 if(content.substring(36,37)== "0")
 {
   digitalWrite(46, HIGH);
   assign(relay4);
   police();
 }
 else if (content.substring(36,37)== "1")
 {
   digitalWrite(46, LOW);
   lowsign(relay4);
 }

 if(content.substring(37,38)== "0")
 {
   digitalWrite(44, HIGH);
   assign(relay5);
 }
 else if (content.substring(37,38)== "1")
 {
   digitalWrite(44, LOW);
   lowsign(relay5);
 }

 content = "";
}


void readsms()
{
      GPRS.print("AT+CMGF=1\r");
      delay(100);
      showdata();
      GPRS.print("AT+CNMI=2,2,0,0,0\r");
      delay(100);
      showdata();      
}

void sendSMS()
{
  // AT command to set SIM900 to SMS mode
  GPRS.print("AT+CMGF=1\r"); 
  delay(100);
  showdata();

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  GPRS.print("AT+CMGS=\""); 
  GPRS.print(Data);
  GPRS.println("\"");
  delay(100);
  showdata();
  // Send the SMS
  GPRS.println("hey"); 
  delay(100);
  showdata();

  // End AT command with a ^Z, ASCII code 26
  GPRS.println((char)26); 
  delay(100);
  showdata();
  GPRS.println();
  // Give module time to send SMS
  delay(5000);
  showdata();  
}

void call()
{
  GPRS.print("ATD"); 
  GPRS.println(Data1);
    delay(1000);
  showdata();  
}

void alarm()
{
  for (int y = 0; y<5 ; y++)
  {
    state = 1;
    if (state == 1)
    {
        for(i = 0; i < 255; i = i + 2)
        {
            analogWrite(LEDFLASHER, i);
            analogWrite(ALARMSOUNDER, i);
            delay(10);
        }
        for(i = 255; i > 1; i = i - 2)
        {
            analogWrite(LEDFLASHER, i);
            analogWrite(ALARMSOUNDER, i);
            delay(5);
        }
        for(i = 1; i <= 10; i++)
        {
            analogWrite(LEDFLASHER, 255);
            analogWrite(ALARMSOUNDER, 200);
            delay(100);
            analogWrite(LEDFLASHER, 0);
            analogWrite(ALARMSOUNDER, 25);
            delay(100);
        }
    }
    else
    {
        analogWrite(ALARMSOUNDER, 0);
        analogWrite(LEDFLASHER, 0);
    }
    pre_val = val;  
  }
}

void police()
{
  for (int i = 0; i < 10; i++)
  {
  for (int freq = 150; freq <1800; freq++)
  {
    tone(26, freq, 10);
    digitalWrite(46, HIGH);
    delay(1);
  }

  for (int freq = 150; freq <1800; freq++)
  {
    tone(26, freq, 10);
    digitalWrite(46, LOW);
    delay(1);
  }
  }
}

void recieveSMS()
{
  GPRS.print("AT+CMGF=1\r"); 
  delay(100);
  showdata();
  // Set module to send SMS data to serial out upon receipt 
  GPRS.print("AT+CNMI=2,2,0,0,0\r");
  delay(2000);
  showdata();

  if(GPRS.available()>0){
    textMessage = GPRS.readString();
    delay(1000);
    GPRS.print(textMessage);    
  } 
  if(textMessage.indexOf("PLEASE")>=0){
    // Turn on relay and save current state
    digitalWrite(44, HIGH);
    GPRS.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("CALL")>=0){
    // Turn off relay and save current state
    digitalWrite(44, HIGH);
    GPRS.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("PLEASE")>=0){
    sendSMS();
    GPRS.println("Lamp state resquest");
    textMessage = "";
  }
  
}
