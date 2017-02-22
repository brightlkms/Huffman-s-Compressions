#include "BitInputStream.h"

using namespace std;

void BitInputStream::fill(){
  // cout << endl;
  buf = in.get(); // read one byte from istream to bitwise buffer
  nbits = 0;      // no bits have been read from bitwise buffer
}

unsigned int BitInputStream::readBit(){
  // fill bitwise buffer if there are no more unread bits
  
  if(in.eof()){
    // cout << "total bit " << tbit <<endl;
    // cout << "remain bit " << remainBit <<endl;
    return -1;
  }
  if(nbits == 8) {
      // cout << "fill" <<endl;
      fill();
  }
  if(in.eof()){
    return -1;
  }
    cout << "total bit " << tbit <<endl;
    // cout << "remain bit " << remainBit <<endl;
  // cout << "remain bit " << remainBit <<endl;
  // cout << "bitshift " << bitshift <<endl;
  if(remainBit == tbit){
    // cout << "total bit " << tbit <<endl;
    cout << "remain bit " << remainBit <<endl;
    cout << "tbit " << tbit <<endl;
    for(int i=0;i<bitshift;i++){
      // cout << "shift" <<endl;
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
  // if(nextBit == 128){
  //   cout << "1";
  // }
  // else{
  //   cout <<"0";
  // }
  return nextBit;
}
void BitInputStream::setTbit(int bits){
  //hard code 512 for 256 ASCI char and new line
  //minus one for first byte indicating shift
  tbit = bits;
  // tbit = filesize;
  // bitshift = bs;
  // remainBit = 8-bitshift;
}
void BitInputStream::setRemainder(int bs){
  //hard code 512 for 256 ASCI char and new line
  //minus one for first byte indicating shift
  remainBit= 8-bs;
  // tbit = filesize;
  bitshift = bs;
  // remainBit = 8-bitshift;
}