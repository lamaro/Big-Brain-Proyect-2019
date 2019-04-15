  #include <Arduino.h>

class Sensor {

  private:    
    static const uint8_t   OFF = 0;
    static const uint8_t   ON = 1;
    static const char DATA_SEP = ':';
    static const char LINE_SEP = '#';
    uint8_t   pin;
    bool      isAnalog;
    bool      isFrecCount;
    uint8_t   state;

    uint16_t  minTh;
    uint16_t  maxTh;
    
    uint16_t  MIN;
    uint16_t  MAX;
    
    uint16_t  localMin;
    uint16_t  localMax;
 
    float  Q;   

    volatile uint16_t currentReading;
    //volatile uint16_t filteredData;
    volatile float normalizedData;

    //volatile unsigned long pulseLength = 0;
    uint8_t pulse;
    bool isUp;
    bool cambioDir;
    
    String name;
    
    /*int avg[10];
    uint16_t count;
    uint16_t AVGSIZE;*/

  public:
    Sensor(uint8_t _pin, bool _isAnalog, bool _isFrecCount, String _name) {
      pin = _pin;
      isAnalog = _isAnalog;
      isFrecCount = _isFrecCount;
      minTh = 10; // default
      Q = 0.99;
      name = _name;
      
      localMin=localMax=512;
      pulse=0;
      isUp=true;
      cambioDir=false;
      //AVGSIZE = sizeof(avg);
      
      if(isAnalog) minTh = 470;
      pinMode(pin, INPUT);
    };

    void setMinMax(uint16_t  _minTh, uint16_t  _maxTh) {
      minTh = _minTh;
      maxTh = _maxTh;
    }
    
    void setMinMaxLimits(uint16_t  _minTh, uint16_t  _maxTh) {
      MIN = _minTh;
      MAX = _maxTh;
    }
    
    void setFactor(float factor) {
      Q=factor;
    }

    void update() {      
      int lastRead = currentReading;
      
      if (isAnalog) {
        currentReading = 1024 - analogRead(pin);
        state = currentReading > minTh ? 1 : 0;
        normalizedData = normalize(ease(currentReading,lastRead,Q),MIN,MAX);
        
//        MAX = currentReading>MAX?currentReading:MAX;
//        MIN = currentReading<MIN?currentReading:MIN;
//
//        if(currentReading<1000&&currentReading<100){
//          if(isUp){
//            if(currentReading>lastRead){
//              localMax=currentReading;
//            }else if(!cambioDir){
//              localMax=lastRead;
//              cambioDir=true;
//            }else if(currentReading<(localMax+localMin)/2){
//                isUp=false;
//                cambioDir=false;
//                pulse=1;                
//            }
//          }else{
//            if(currentReading<lastRead){
//              localMin=currentReading;
//            }else if(!cambioDir){
//              localMin=lastRead;
//              cambioDir=true;
//            }else if(currentReading>(localMax+localMin)/2){
//                isUp=true;
//                cambioDir=false;
//                pulse=0;
//            }          
//          }
//        }else{
//         localMin=512; 
//         localMax=512;
//        }
        
        
      } else {
        if (isFrecCount) {
          // low pass filtering
//          currentReading = easeUint(pulseIn(pin, HIGH, 1000L), currentReading, 3);
          currentReading = pulseIn(pin, HIGH, 1000L);
          state = (currentReading > minTh) ? 1 : 0;
          normalizedData = normalize(ease(currentReading,lastRead,Q),MIN,MAX);          
        } else {
          // reseteo lo que dejo la interrupcion
          if(currentReading > 0) currentReading--; // easeUint(0, currentReading, 3);
          normalizedData = normalize(ease(currentReading,lastRead,Q),MIN,MAX);          
          state = currentReading > minTh ? 1 : 0;
        }
      }
    };

    int debug() {
      Serial.print("pin:");
      Serial.print(pin);
      Serial.print("\t|");
      Serial.print("state: ");
      Serial.print(state);
      Serial.print("\t|");
      Serial.print("current value: ");
      Serial.print(currentReading);
      Serial.println("\t| ");
    };
    
    void serialWrite() {
      Serial.print(LINE_SEP);
      Serial.print(name);
      Serial.print(DATA_SEP);
      Serial.print(pin);
      Serial.print(DATA_SEP);
      Serial.print(currentReading);
      Serial.print(DATA_SEP);
      Serial.print(normalizedData);
      Serial.print(DATA_SEP);
      Serial.print(state);
      Serial.print(DATA_SEP);
      Serial.print(pulse);
    };

    uint16_t easeUint(uint16_t current, uint16_t prev, uint16_t percent) {
      return  (current * 100 / percent + prev * (100 - (100 / percent))) / 100;
    };
    
    float ease(uint16_t current, uint16_t prev, float factor) {
      return  ((current * factor) + (prev * 1.0 * (1.0 - factor)));
    };
    
    float normalize(float val, uint16_t _min , uint16_t _max) {
      return  (val - _min) * 1.0 / (_max-_min);
    };

    void handleInterrupt() {
      currentReading = 255;
    };

    /*void getMinMax(){
        MIN=512;
        MAX=512;
       for(int i=0;i<AVGSIZE;i++){
          MAX = avg[i]>MAX?avg[i]:MAX;
          MIN = avg[i]<MIN?avg[i]:MIN;           
       }
    }*/


};
