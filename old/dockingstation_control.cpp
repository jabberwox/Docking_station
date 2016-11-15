#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

//int wiringPiSetupSys (void)
//{
//  for (int i=1; i<40; i++) {
//  pinMode(i, OUTPUT);
//  }
//}


int main (void)
{
  if (wiringPiSetupSys() == -1) return 1;
//  system("gpio unexportall");
//  system("gpio export 13 out");
//  system("gpio export 23 out");
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
return 0;
}
