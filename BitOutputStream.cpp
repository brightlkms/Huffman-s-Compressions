#include "BitOutputStream.h"

using namespace std;

void BitOutputStream::flush() {
  cout << endl;
  out.put(buf);  // write the bitwise buffer to the ostream
  out.flush();   // flush the ostream (optional, slower to do it here)
  buf = 0;       // clear the bitwise buffer
  nbits = 0;     // bitwise buffer is cleared, so there are 0 bits in it
}
int BitOutputStream::returnNbits(){
  return nbits;
}
void BitOutputStream::writeBit(unsigned int bit) {
  cout << bit;
  // cout << "writing bit: " << bit << endl;
  // flush the bitwise buffer if it is full
  // set the next open bit of the bitwise buffer to 'bit' (how?)
  //case for last byte
  // cout << "remain Bit: " << remainBit <<endl;
  // cout << "my bit: "<< bit <<endl;
  // cout << "nbits " << nbits <<endl;
  // cout << "tbit: " << tbit << endl;
  if(remainBit == tbit){
    // cout << "did this happen" <<endl;
    for(int i=0; i < bitshift; i++){
      // cout << "what up cap"<<endl;
      buf = (buf << 1); 
      cout<<'0';
      nbits++;
      // cout << "ge" <<endl;
      if(nbits == 8) {
        // cout << "ge" <<endl;
        flush();
      }
    }
  }
  if(bit == 1)
    buf = (buf << 1) | 1;
  else if(bit == 0)
    buf = (buf << 1); 
  //what happens when we run into case where you don't have to perfectly 8 bits
  // increment the number of bits in our bitwise buffer
  nbits++;
  tbit--;
  // cout << "tbits is" << tbit <<endl;
  if(nbits == 8) {
    flush();
  }
}
void BitOutputStream::setRemainBit(int tb, int bs){
  tbit = tb;
  bitshift = bs;
  remainBit = 8-bs;
}