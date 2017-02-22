#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <fstream>
#include <iostream>
using namespace std;

class BitOutputStream {
    private:
        unsigned char buf; // bitwise buffer (one byte)
        int nbits;         // number of bits that have been written to the bitwise buffer
        ostream & out;     // reference to the bytewise output stream (a C++ ostream)

        //tbit is total used for tracking whether or not it is composing the last byte
        int tbit;
        //bitshift indicates how much you need to pad
        int bitshift;
        //remainBit used for signaling writebit that it's about to compose the last byte
        int remainBit;

    public:
        // constructor: assign 'out' to 'os', 'buf' to 0, and 'nbits' to 0
        BitOutputStream(ostream & os) : out(os), buf(0), nbits(0) {}

        // send the bitwise buffer to the output stream, and clear the bitwise buffer
        void flush();

        // write bit to the bitwise buffer
        void writeBit(unsigned int bit);

        void setRemainBit(int, int);
        int returnNbits();
};
#endif