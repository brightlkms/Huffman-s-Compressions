#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <fstream>
#include <iostream>
using namespace std;

class BitInputStream {
    private:
        unsigned char buf;  // bitwise buffer (one byte)
        int nbits;          // number of bits that have been read from bitwise buffer
        istream & in;       // reference to the bytewise input stream (a C++ istream)

        //tbit is total used for tracking whether or not it is composing the last byte
        int tbit;
        //bitshift indicates how much you need to pad
        int bitshift;
        //remainBit used for signaling writebit that it's about to compose the last byte
        int remainBit;
    public:
        // constructor: assign 'in' to 'is', 'buf' to 0, and 'nbits' to 8
        BitInputStream(istream & is) : in(is), buf(0), nbits(8) {}

        // fill the bitwise buffer by reading one byte from the input stream
        void fill();

        // read bit from the bitwise buffer
        unsigned int readBit();

        //track remaining bit
        void trackRemainBit(int, int);
};
#endif