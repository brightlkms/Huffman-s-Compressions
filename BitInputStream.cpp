/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: An implementation file for BitInputStream. Functions include readBit that reads the next bit in the bitwise buffer. Once 8 bits are read, function fill gets called to grab the next byte. setTbit can set tell inputstream the totalbit that will be decoded. The setRemainder function serves the purpose of setting bitshift and remainder bit for bitinputstream object. This is for shifting the last bit appropriately
*/
#include "BitInputStream.h"

using namespace std;

void BitInputStream::fill(){
  buf = in.get(); // read one byte from istream to bitwise buffer
  nbits = 0;      // no bits have been read from bitwise buffer
}

unsigned int BitInputStream::readBit(){
  if(in.eof() || tbit==0){
    return -1;
  }
  if(nbits == 8) {
      fill();
  }
  if(bitshift == tbit){
    for(int i=0;i<remainBit;i++){
      buf=(buf<<1); 
      nbits++;
    }
    return -1;
  }
  else{
    int nextBit = buf & (1 << 7);
    buf=(buf<<1);
    nbits++;
    tbit--;
    return nextBit;
  }
}
void BitInputStream::setTbit(int bits){
  //hard code 512 for 256 ASCI char and new line
  //minus one for first byte indicating shift
  tbit = bits;
}
void BitInputStream::setRemainder(int bs){
  //hard code 512 for 256 ASCI char and new line
  //minus one for first byte indicating shift
  remainBit= 8-bs;
  bitshift = bs;
}