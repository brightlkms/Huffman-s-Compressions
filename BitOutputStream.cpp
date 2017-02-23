/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: An implementation file for BitOutputStream. Functions include writeBit that writes the next bit in the bitwise buffer. Once 8 bits are written, function flush gets write out the 8 bits to the out stream file.
*/
#include "BitOutputStream.h"

using namespace std;

void BitOutputStream::flush() {
  // cout << "hey";
  out.put(buf);  
  out.flush();   
  buf = 0;       
  nbits = 0;     
}
int BitOutputStream::returnNbits(){
  return nbits;
}
void BitOutputStream::writeBit(unsigned int bit, bool doshift) {
  if(doshift){
    for(int i=0; i < bitshift; i++){
      buf = (buf << 1); 
      // cout<<'0';
      nbits++;
      if(nbits == 8) {
        flush();
      }
    }
  }
  else{
    if(bit == 1)
      buf = (buf << 1) | 1;
    else if(bit == 0)
      buf = (buf << 1); 
    nbits++;
    tbit--;
    if(nbits == 8) {
      flush();
    }
  }
}
void BitOutputStream::setRemainBit(int tb, int bs){
  tbit = tb;
  bitshift = bs;
  remainBit = 8-bs;
}
