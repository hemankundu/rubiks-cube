#include <AFMotor.h>
#include <AccelStepper.h>
double ROTATE=54, FLDIST=120, SPIN=50, ROT=57;
String cmd, cmdstr;
bool readDone=false;

AF_Stepper baseMotor(200, 1);
AF_Stepper flipMotor(200, 2);


void setup() {
  baseMotor.setSpeed(80);
  flipMotor.setSpeed(40);
  Serial.begin(9600);
  cmd.reserve(6);
  cmdstr.reserve(1500);
  cmdstr="";
  Serial.println("Serial started!");
}

void RotateBaseMotor(double angle, bool dir){
  baseMotor.step(angle, (dir)?FORWARD:BACKWARD, SINGLE);
  baseMotor.release();
  delay(200);
}

void Flip(){
  double angle=60;
  // baseMotor.step(1, FORWARD, SINGLE); 
  // delay(20);
  // baseMotor.step(1, BACKWARD, SINGLE);  
  flipMotor.step(angle, FORWARD, SINGLE);
  flipMotor.release();
  delay(500);
  flipMotor.step(angle, BACKWARD, SINGLE);
  flipMotor.release();
  //baseMotor.release();
  delay(200);
}
void Lock(bool lc){
  double angle=22;
  // baseMotor.step(1, FORWARD, SINGLE); 
  // delay(20);
  // baseMotor.step(1, BACKWARD, SINGLE);  
  if(lc){
    //Lock
    flipMotor.step(angle, FORWARD, SINGLE);
    flipMotor.release();
  }else{
    //Unlock
    flipMotor.step(angle, BACKWARD, SINGLE);
    flipMotor.release();
    RotateBaseMotor(1,false);
  }
  //baseMotor.release();
  delay(200);
}
int loopcnt=1;

void loop(){
  while(loopcnt--){
  ReadFromSerial();
  if(readDone){
    readDone=false;
    Process();
    cmdstr="";
  }
    //Process();
  }
}

void ReadFromSerial(){
  char ch;
  while(!Serial.available());
  while(Serial.available()){
    ch=(char)Serial.read();
    if(ch=='>'){
      cmdstr += ch;
      readDone=true;
      break;
    }else{
      cmdstr += ch;
    }
  }
}

void Process(){
  Serial.println(cmdstr);
  //Serial.flush();
  int i,cnt=0;
  cmd+=cmdstr[0];
  cmd+=cmdstr[1];
  cnt=(int)cmdstr[2]-48;
  //perform
  if(cmd=="FL"){
    while(cnt--){
      Flip();
      delay(300);
    }
  }else if(cmd=="LC"){
    Lock(true);
  }else if(cmd=="UL"){
    Lock(false);
  }else if(cmd=="RO"){
    RotateBaseMotor(cnt*ROT, true);
  }else if(cmd=="RC"){
    RotateBaseMotor(cnt*(ROT-4), false);
  }else if(cmd=="SP"){
    RotateBaseMotor(cnt*SPIN, true);
    //RotateBaseMotor(3, false);
  }else if(cmd=="SC"){
    RotateBaseMotor(cnt*SPIN, false);
  }
  cmd="";
  cnt=0;
  cmdstr="";
  loopcnt++;
}