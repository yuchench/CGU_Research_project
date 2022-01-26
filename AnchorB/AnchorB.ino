#include <SPI.h>
#include <DW1000.h>
#include "DW1000Ranging.h"
#include <Timer.h>

Timer tcb;
String rangeNum;

const int COUNT=10;  
double arr1[COUNT]={};
double result=0.00;
double resultcm=0.00;
double totalR=0.00;
double range1=1.00;
int jo=0;

const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

boolean sent = false;
volatile boolean sentAck = false;
volatile unsigned long delaySent = 0;
int16_t sentNum = 0; // todo check int type
DW1000Time sentTime;

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
      tcb.every(3000,senderTest);
}

void handleSent() {
  sentAck = true;
} 

void transmitter() {
  DW1000.newTransmit();
  DW1000.setDefaults();
  rangeNum="B  ";
  String msg = rangeNum;
  String msg2 = msg + range1;
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
  resultcm=result*100;
  if(jo==0){
    arr1[0]=resultcm;
    jo++;
  }else if(jo==1){
    arr1[1]=resultcm;
    jo++;
  }else if(jo==2){
    arr1[2]=resultcm;
    jo++;
  }else if(jo==3){
    arr1[3]=resultcm;
    jo++;
  }else if(jo==4){
    arr1[4]=resultcm;
    jo++;
  }else if(jo==5){
    arr1[5]=resultcm;
    jo++;
  }else if(jo==6){
    arr1[6]=resultcm;
    jo++;
  }else if(jo==7){
    arr1[7]=resultcm;
    jo++;
  }else if(jo==8){
    arr1[8]=resultcm;
    jo++;
  }else if(jo==9){
    arr1[9]=resultcm;
    totalR=(arr1[0]+arr1[1]+arr1[2]+arr1[3]+arr1[4]+arr1[5]+arr1[6]+arr1[7]+arr1[8]+arr1[9])/10;
    range1=totalR;    
    Serial.println(range1);
    jo=0; 
  }
}
