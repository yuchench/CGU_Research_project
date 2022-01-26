#include <SPI.h>
#include <DW1000.h>
#include "DW1000Ranging.h"
#include <Timer.h>

//----------------------Setting all the require variables----------------------
Timer tcb;
String rangeNum;

const int COUNT=10;  
double arr1[COUNT]={};
double result=0.00;
double totalR=0.00;
double range1=1.00;
boolean countingT=true;
int jo=0;

//----------------------Setup the Arduino equuipments' variables-------------------
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

//-----------------------background information control----------------------------
boolean sent = false;
volatile boolean sentAck = false;
volatile unsigned long delaySent = 0;
int16_t sentNum = 0; 
DW1000Time sentTime;

//-----------------------Before real transmission, testing all the connection------
void anchorTest(){
      DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); 
      DW1000Ranging.attachNewRange(newRange);
      DW1000Ranging.startAsAnchor("Anchor Start ", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
}

void senderTest(){
       DW1000.begin(PIN_IRQ, PIN_RST);
       DW1000.select(PIN_SS);
       DW1000.newConfiguration();
       DW1000.setDefaults();
       DW1000.setDeviceAddress(5);
       DW1000.setNetworkId(10);
       DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
       DW1000.commitConfiguration();   
       char msg[128];
       DW1000.getPrintableDeviceIdentifier(msg);
       DW1000.attachSentHandler(handleSent);   
       transmitter();
}
void setup() {
      Serial.begin(115200);
      tcb.every(1000,anchorTest);
      tcb.every(2000,senderTest);
}

void handleSent() {
  sentAck = true;
} 

void transmitter() {
  DW1000.newTransmit();
  DW1000.setDefaults();
  rangeNum="X  "; //-----------------------------------------------decide which anchor will be activate----------------------------------------
  String msg = rangeNum;
  String msg2=msg+range1;
  DW1000.setData(msg2);  
  DW1000.startTransmit();
}
void loop() {
  tcb.update();
  DW1000Ranging.loop();
  if (!sentAck) {
      return;
  }
  sentAck = false;   
    delay(10);
    transmitter();  
}
void newRange() { 
  result=DW1000Ranging.getDistantDevice()->getRange();
  result=result*100;
  if(jo==0){
    arr1[0]=result;
    jo++;
  }else if(jo==1){
    arr1[1]=result;
    jo++;
  }else if(jo==2){
    arr1[2]=result;
    jo++;
  }else if(jo==3){
    arr1[3]=result;
    jo++;
  }else if(jo==4){
    arr1[4]=result;
    jo++;
  }else if(jo==5){
    arr1[5]=result;
    jo++;
  }else if(jo==6){
    arr1[6]=result;
    jo++;
  }else if(jo==7){
    arr1[7]=result;
    jo++;
  }else if(jo==8){
    arr1[8]=result;
    jo++;
  }else if(jo==9){
    arr1[9]=result;
    totalR=(arr1[0]+arr1[1]+arr1[2]+arr1[3]+arr1[4]+arr1[5]+arr1[6]+arr1[7]+arr1[8]+arr1[9])/10;
    range1=totalR;    
    Serial.println(range1);
    /*
    if(totalR<=44.0){
      totalR=totalR-29;
      range1=totalR;
    }else if(totalR<=48){
      totalR=totalR-28;
      range1=totalR;
    }else if(totalR<=52.6){
      totalR=totalR-27.6;
      range1=totalR;
    }else if(totalR<=55.8){
      totalR=totalR-25.8;
      range1=totalR;
    }else if(totalR<=59){
      totalR=totalR-24;
      range1=totalR;
    }else if(totalR<=66.8){
      totalR=totalR-26.8;
      range1=totalR;
    }else if(totalR<=78.8){
      totalR=totalR-33.8;
      range1=totalR;
    }else if(totalR<=77.6){
      totalR=totalR-27.6;
      range1=totalR;
    }else if(totalR<=82.5){
      totalR=totalR-27.5;
      range1=totalR;
    }else if(totalR<=91){
      totalR=totalR-31;
      range1=totalR;
    }else if(totalR<=95.2){
      totalR=totalR-30.2;
      range1=totalR;
    }else if(totalR<=103){
      totalR=totalR-33;
      range1=totalR;
    }else if(totalR<=107.8){
      totalR=totalR-32.8;
      range1=totalR;
    }else if(totalR<=112.8){
      totalR=totalR-32.8;
      range1=totalR;
    }else if(totalR<=121){
      totalR=totalR-36;
      range1=totalR;
    }else if(totalR<=126){
      totalR=totalR-36;
      range1=totalR;
    }else if(totalR<=131.6){
      totalR=totalR-36.6;
      range1=totalR;
    }else if(totalR<=134.6){
      totalR=totalR-34.6;
      range1=totalR;
    }else{
      totalR=totalR-36.0;
      range1=totalR;
    } 
    */   
    countingT=false;
    jo=0;
  }
}
