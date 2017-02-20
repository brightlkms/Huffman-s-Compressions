#include "BitInputStream.h"

using namespace std;

void BitInputStream::fill(){
  buf = in.get(); // read one byte from istream to bitwise buffer
  nbits = 0;      // no bits have been read from bitwise buffer
}

unsigned int BitInputStream::readBit(){
  // fill bitwise buffer if there are no more unread bits
  // cout << "total bit " << tbit <<endl;
  // cout << "remain bit " << remainBit <<endl;
  if(nbits == 8) {
      fill();
  }
  if(remainBit == tbit){
    for(int i=0;i<bitshift;i++){
      buf=(buf<<1);
      nbits++;
    }
  }
  // get the next unread bit from the bitwise buffer (take most left in the byte)
  int nextBit = buf & (1 << 7);
  // cout << "nextBit: "<< nextBit <<endl;
  // cout << "before shift expect(137)     " << int(buf) <<endl;
  buf=(buf<<1);
  // cout << "after shift expect(18)     " << int(buf) <<endl;
  // increment the number of bits we have read from the bitwise buffer
  nbits++;
  tbit--;
  // return the bit we just read
  return nextBit;
}
void BitInputStream::trackRemainBit(int filesize, int bs){
  //hard code 512 for 256 ASCI char and new line
  //minus one for first byte indicating shift
  tbit = (((filesize-512)-1)*8);
  bitshift = bs;
  remainBit = 8-bitshift;
}