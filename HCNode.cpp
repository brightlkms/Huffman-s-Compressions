/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: This is an implementation file for Huffman Compression Node Class.
*/
#include "HCNode.h"

//lower count means higher priority
bool HCNode::operator<(const HCNode& other){
	//if counts are different, just compare counts
	if(count != other.count) 
		return count > other.count;

	//counts are equal. use symbol value to break tie.
	//(for this to work, interanl HCNodes must have 
	//symb set.
	return symbol < other.symbol;
}