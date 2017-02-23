#include "BitInputStream.h"

using namespace std;

void BitInputStream::fill(){
  buf = in.get(); // read one byte from istream to bitwise buffer
  nbits = 0;      // no bits have been read from bitwise buffer
}

unsigned int BitInputStream::readBit(){
  if(in.eof()){
    return -1;
  }
  if(nbits == 8) {
      fill();
  }
  if(tbit==0){
    return -1;
  }
  if(bitshift == tbit){
    for(int i=0;i<remainBit;i++){
      // cout << "shift" <<endl;
      buf=(buf<<1); 
      nbits++;
    }
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