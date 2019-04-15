#include "Sensor.cpp"

uint8_t temp    = A0;
uint8_t mic     = A4;
uint8_t cap     = A2;
uint8_t ligth   = A3;

Sensor tempSensor(temp, true, false, "t");
Sensor micSensor(mic, true, false, "m");
Sensor capSensor(cap, false, true, "c");
Sensor ligthSensor(ligth, true, false, "l");

static const char DATA_SEP = ':';
static const char LINE_SEP = '#';

String command;
String args;
bool newCom = true;

void setup() {
  Serial.begin(9600);  
  tempSensor.setMinMaxLimits(450,520);
  micSensor.setMinMaxLimits(0,255);
  
  capSensor.setMinMaxLimits(0,100);
  capSensor.setFactor(0.1);
  
  ligthSensor.setMinMaxLimits(100, 1024);
  attachInterrupt(0, isr, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(33);
  tempSensor.update();
  //tempSensor.debug();
  micSensor.update();
 //micSensor.debug();
  capSensor.update();
  ligthSensor.update();
  
  tempSensor.serialWrite();
  micSensor.serialWrite();
  capSensor.serialWrite();
  ligthSensor.serialWrite();
  
}

void isr(){
  micSensor.handleInterrupt();
 // Serial.println("____________________");
}


//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read(); 
//    // add it to the inputString:
//    if (inChar != LINE_SEP) {
//        if (inChar != DATA_SEP && newCom){
//              command += inChar;       
//        }else if(newCom){
//             newCom=false;             
//        }else{
//             args+=inChar; 
//        }
//    }else{        
//       setSensor(command,args);
//    }
//  }
//}

//void setSensor(String com, String a){
//   Sensor s = getSensor(command[1]);
//   if(command[0]=='M'){
//    int argu[2];
//    getArgs(args,argu);
//    s.setMinMax(argu[0],argu[1]);
//   }else if(command[0]=='Q'){
//    int argu[1];
//    getArgs(args,argu);
//    s.setFactor(argu[0]);
//   }
//      
//  
//}

//Sensor getSensor(int pin){
//    switch (pin) {
//    case 1:
//      return pulgarSensor;
//    case 2:
//      return tempSensor;
//    case 3:
//      return torSensor;
//    
//  }
//}

void getArgs(String a, int argu[]){
      //int argu[cant];
      String temp = "";
      int cont=0;
      for(int i=0;i<sizeof(args)-1;i++){
        if(args[i]==DATA_SEP){
            argu[cont]=temp.toInt();
            cont++;            
        }
        temp+=args[i];
      }      
}

