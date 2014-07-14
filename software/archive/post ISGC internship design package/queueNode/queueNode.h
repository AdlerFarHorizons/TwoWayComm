/*
  This is a header file for a node class that will serve to define data bursts


*/

#ifndef _queueNode_H_INCLUDED
#define _queueNode_H_INCLUDED

#include <stdio.h>
#include <Arduino.h>
#include <pins_arduino.h>

class Node {
public:
byte * content;
int size;

};

#endif