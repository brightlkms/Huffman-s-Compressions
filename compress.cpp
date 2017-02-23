/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: Compress program takes in two arguemnts, first argument is the file that compress will encode and second argument is the out file where the encoded objects will go. Each helper function description is written above the function protocol.
*/
#include "HCTree.h"
#include "HCNode.h"
#include <math.h>

using namespace std;

//Binary function takes in int and BitOutputStream as a reference. It takes in the integer number writes equivalent binary bit to outputstream
void binary(int, BitOutputStream&);

//file_size function takes in name of an infile as string and return the size in byte
unsigned int file_size(string);

//get_twobit takes in HCNode* pointer and bitoutputstream. It will find what type of children that node has. 00- both children node are leafs. 01 - left child node is a child. 10 - right child node is a leaf. 11 - both children nodes are not leaf nodes
int get_twobit(HCNode*, BitOutputStream&);

//traverse function takes in HCNode pointer, bitoutputstream and ofstream. It recursively encodes to a file depending on the command that was returned from get_twobit
void traverse(HCNode*, BitOutputStream&, ofstream&);


int main(int argc, char** argv){

	cout << "Compression Running" <<endl;
	if(argc < 2){
		cout << "Compress requires 2 arguments infile and outfile" <<endl;
		return -1;
	}
	string infile = argv[1];
	string outfile = argv[2];

	int c;
	ifstream in;
	in.open(infile);
	unsigned char next;
	vector<int> freqs (256, 0);

	if(in.peek() == ifstream::traits_type::eof()){
		cout<<"File is empty";
		return -1;
	}
	while(1){
		next = in.get();//get one byte
		if(in.eof()) break;
		else{
			c=next;
			freqs[c]++;
		}
	}
	//Constructing Huff man
	HCTree hct;
	hct.build(freqs);
	//Find amount of unique nodes
	int leafs=0;
	for(auto a: freqs){
		if(a!=0){
			leafs++;
		}
	}
	//------------------Final starts here-----------------------
	ofstream out;
	out.open(outfile);
	BitOutputStream bos(out);
	
	in.clear();
	in.seekg(0, ios::beg);
	if(leafs == 1){
		int i=0;
		int freq, asci;
		
		for(auto a : freqs){
			if(a!=0){
				freq=a;//freq
				asci=i;//index
			}
			i++;
		}
		int bitshift = (8-(freq%8));
		binary(leafs, bos);//write leaf
		binary(bitshift, bos);//write bitshift
		binary(asci, bos);//write bitshift
		bos.setRemainBit(0, bitshift);
		bos.writeBit(1, true);//shiftbits
		
		for(int i=0;i<freq;i++){
			bos.writeBit(1, false);
		}
	}
	else{
		binary(leafs, bos);
		//ENCODE MAGICAL HEADER
		//-----------------------------------------------------
		traverse(hct.return_root(), bos, out);
		//-----------------------------------------------------
		
		hct.totalbits=0;
		//temp holds the number of bits there will be for content in the file excluding everyelse
		int temp;
		while(1){
			next = in.get();
			if(in.eof()) break;
			hct.find_future(next);
		}
		temp = hct.totalbits;
		unsigned int headerSize = file_size(outfile);
		int real_bitshift = 8-((temp+bos.returnNbits())%8);

		in.clear();
		in.seekg(0, ios::beg);

		bos.setRemainBit(temp+8, real_bitshift);
		binary(real_bitshift, bos);

		in.clear();
		in.seekg(0, ios::beg);
		//ENCODE CHAR
		while(1){
			next = in.get();
			if(in.eof()) break;
			hct.encode(next, bos);
		}
		bos.writeBit(0, true);
	}
	//-----------------------------------------------------
	out.close();
	in.close();
	return 0;
}
void traverse(HCNode* ptr, BitOutputStream& bos, ofstream& out){
	binary(ptr->symbol, bos);
	int command = get_twobit(ptr, bos);

	if(command == 1){
		binary(ptr->c0->symbol, bos);
	}
	else if(command == 2){
		traverse(ptr->c0, bos, out);
	}	
	else if(command == 3){
		binary(ptr->c0->symbol, bos);
		traverse(ptr->c1, bos, out);
	}
	else if(command == 4){
		traverse(ptr->c1, bos, out);
		traverse(ptr->c0, bos, out);
	}
}
int get_twobit(HCNode* ptr, BitOutputStream& bos){
	if(ptr->c1->c1==0 && ptr->c0->c1==0){
		bos.writeBit(0, false);
		bos.writeBit(0, false);
		return 1;
	}
	else if(ptr->c1->c1==0 && ptr->c0->c1!=0){
		bos.writeBit(0, false);
		bos.writeBit(1, false);
		return 2;
	}
	else if(ptr->c1->c1!=0 && ptr->c0->c1==0){
		bos.writeBit(1, false);
		bos.writeBit(0, false);
		return 3;
	}
	//11
	else if(ptr->c1->c1!=0 && ptr->c0->c1!=0){
		bos.writeBit(1, false);
		bos.writeBit(1, false);
		return 4;
	}
	else{
		return -1;
	}
}
unsigned int file_size(string filename){
	streampos begin,end;
	ifstream myfile (filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg (0, ios::end);
	end = myfile.tellg();
	myfile.close();

	unsigned int size = end-begin;
	return size;
}

void binary(int task, BitOutputStream& bos){
	int count = 7;
	int cur = pow(2, count);
	vector<int> bits;

	while(count > -1 | task!=0){
		cur = pow(2, count);
		if(task-cur > 0){
			bits.push_back(1);
			count--;
			task-=cur;
		}	
		else if(task-cur < 0){
			bits.push_back(0);
			count--;
		}
		else if(task-cur == 0){
			bits.push_back(1);
			task-=cur;
			for(int i=0; i<count;i++){
				bits.push_back(0);
				count--;
			}
			count--;
		}
	}
	for(auto a: bits){
		bos.writeBit(a, false);
	}
}