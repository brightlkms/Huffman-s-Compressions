/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: Uncompress program takes in two arguemnts, first argument is the infile that uncompress will read from and second argument is the outfile where the encoded objects will get decoded. Each helper function description is written above the function protocol.
*/
#include "HCNode.h"
#include "HCTree.h"
#include <math.h>

using namespace std;

//takes in string of the name of infile and returns the size of the file in bytes
unsigned int file_size(string);
//traverse takes in HCNode pointer, bit input stream object, in stream and int reference. It recursivesly decode encoded bytes. 
void traverse(HCNode*, BitInputStream&, ifstream&, int&);
//Binary function takes in bitinputstream object as a reference. It will read the next 8 bits and find the decimal equivalence of the given byte. 
int binary(BitInputStream&);

int main(int argc, char** argv){
	cout << "Uncompress Running" <<endl;
	if(argc < 2){
		cout << "Compress requires 2 arguments infile and outfile" <<endl;
		return -1;
	}
	string infile = argv[1];
	string outfile = argv[2];

	ifstream in;
	in.open(infile);
	unsigned char next;
	//--------------------
	//DECODE HEADER
	HCTree hct;
	BitInputStream bis(in);

	int size = file_size(infile)*8; 
	bis.setTbit(size);
	int bitshift;
	hct.set_root();

	//--------------------
	ofstream out;
	out.open(outfile);

	int leafs = binary(bis);

	if(leafs==1){
		bitshift = binary(bis);
		bis.setRemainder(bitshift);
		int asci = binary(bis);

		while(bis.readBit() != -1){
			out.put(asci);
		}
	}
	else{
		traverse(hct.return_root(), bis, in, leafs);

		
		//DECODE PADDING
		bitshift = binary(bis);
		bis.setRemainder(bitshift);
		int r;
		unsigned char ch;
		
		//DECODE CH
		while(1){
			r = hct.decode(bis);
			ch = r;
			if (r == -1){
				break;
			}
			else{
				out.put(ch);
			}
		}
	}
	in.close();
	out.close();
}
void traverse(HCNode* ptr, BitInputStream& bis, ifstream& in, int& leaf){
	// write ASCI symbol to file
	unsigned char nextCh = binary(bis);

	int leftc = bis.readBit();
	int rightc = bis.readBit();

	if (leftc==-1 || rightc == -1 || leaf==0){
		return;
	}
	else{
		if(ptr->symbol == 0){
			ptr->symbol = nextCh;
		}
		if(leftc==0 && rightc==0){

			HCNode* leftchild = new HCNode(1, ptr->symbol);
			ptr->c1 = leftchild;

			nextCh = binary(bis);//this is next asci for right child

			HCNode* rightchild = new HCNode(1, nextCh);
			ptr->c0 = rightchild;

			leaf-=2;
		}
		else if(leftc==0 && rightc==128){				
			HCNode* leftchild = new HCNode(1, ptr->symbol);
			ptr->c1 = leftchild;

			leaf-=1;
			HCNode* rightchild = new HCNode(1, 0);
			ptr->c0 = rightchild;		
			traverse(ptr->c0, bis, in, leaf);
		}
		else if(leftc==128 && rightc==0){
			HCNode* leftchild = new HCNode(1, 0);
			ptr->c1 = leftchild;

			nextCh = binary(bis);

			HCNode* rightchild = new HCNode(1, nextCh);
			ptr->c0 = rightchild;
			leaf-=1;
			traverse(ptr->c1, bis, in, leaf);
		}
		else if(leftc==128 && rightc==128){	
			HCNode* leftchild = new HCNode(1, 0);
			ptr->c1 = leftchild;

			HCNode* rightchild = new HCNode(1, 0);
			ptr->c0 = rightchild;		
			
			traverse(ptr->c1, bis, in, leaf);
			traverse(ptr->c0, bis, in, leaf);
		}
	}
}
int binary(BitInputStream& bis){
	int count = 7;
	int total = 0;
	for(int i=0;i<8;i++){
		if(bis.readBit()==128){
			int cur = pow(2, count);
			total+=cur;
		}
		count--;
	}
	return total;
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