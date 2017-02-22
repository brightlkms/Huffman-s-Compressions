#include "HCNode.h"
#include "HCTree.h"
#include <math.h>

using namespace std;

unsigned int file_size(string);
void traverse(HCNode*, BitInputStream&, ifstream&, int&);
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
	// hct.totalbits = size*8;
	//for counting tbit difference
	int size = file_size(infile)*8;
	cout << "file size: " << size <<endl;
	bis.setTbit(size);

	hct.set_root();
	int leafs = binary(bis);
	cout << "minus 8 " <<endl;
	traverse(hct.return_root(), bis, in, leafs);
	cout << "minus header " <<endl;
	//--------------------
	ofstream out;
	out.open(outfile);
	
	//DECODE PADDING
	// int size = (file_size(infile)*8) - count_t -16;
	// cout << size << endl;
	int bitshift = binary(bis);
	cout << "minus 8 " <<endl;
	bis.setRemainder(bitshift);

	// bis.trackRemainBit(size, bitshift);
	//hard cod total but for tree need to come back and fix this
	// hct.totalbits = (((size-512)-1)*8);
	cout << "minus char encode " <<endl;
	// //DECODE CH
	while(1){
		char ch = hct.decode(bis);
		cout << ch <<endl;
		if (ch != -1){
			out << ch;
		}
		else
			break;
	}
	cout << "minus char encode " <<endl;
	in.close();
	out.close();
}
void traverse(HCNode* ptr, BitInputStream& bis, ifstream& in, int& leaf){
	// cout << ptr->symbol <<endl;
	// cout << ptr->symbol <<endl;
	// write ASCI symbol to file
	unsigned char nextCh = binary(bis);
	// count+=8;
	// cout << "next char is " << int(nextCh) <<endl;
	// cout << nextCh <<endl;
	int leftc = bis.readBit();
	int rightc = bis.readBit();
	// count+=2;

	if (leftc==-1 || rightc == -1 || leaf==0){
		return;
	}
	else{
		if(ptr->symbol == 0){
			ptr->symbol = nextCh;
		}
		if(leftc==0 && rightc==0){
			// cout << "twice" <<endl;
			HCNode* leftchild = new HCNode(1, ptr->symbol);
			ptr->c1 = leftchild;
			cout << "c1: " << leftchild->symbol << endl;
			nextCh = binary(bis);//this is next asci for right child
			// count++;
			// cout << "next char is " << int(nextCh) <<endl;
			HCNode* rightchild = new HCNode(1, nextCh);
			ptr->c0 = rightchild;
			cout << "c0: " << rightchild->symbol << endl;
			// return leaf-1;
			leaf-=2;
		}
		else if(leftc==0 && rightc==128){				
			HCNode* leftchild = new HCNode(1, ptr->symbol);
			ptr->c1 = leftchild;
			cout << "c1: " << leftchild->symbol << endl;
			leaf-=1;
			HCNode* rightchild = new HCNode(1, 0);
			ptr->c0 = rightchild;		
			traverse(ptr->c0, bis, in, leaf);
			// return leaf;
		}
		else if(leftc==128 && rightc==0){
			HCNode* leftchild = new HCNode(1, 0);
			ptr->c1 = leftchild;

			nextCh = binary(bis);
			// count++;
			// cout << "next char is " << int(nextCh) <<endl;
			HCNode* rightchild = new HCNode(1, nextCh);
			ptr->c0 = rightchild;
			leaf-=1;
			traverse(ptr->c1, bis, in, leaf);
			cout << "c0: " << rightchild->symbol << endl;
		}
		else if(leftc==128 && rightc==128){	
			// cout << "11" << endl;
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
	// cout << task << endl;
	int count = 7;
	int total = 0;
	for(int i=0;i<8;i++){
		if(bis.readBit()==128){
			int cur = pow(2, count);
		// bits.push_back(bis.readBit());
			total+=cur;
		}
		count--;
	}
	// cout << "total" << total << endl;
	return total;
}

unsigned int file_size(string filename){
	streampos begin,end;
	ifstream myfile (filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg (0, ios::end);
	end = myfile.tellg();
	myfile.close();
	// cin << "size is: " << (end-begin) << " bytes.\n";
	unsigned int size = end-begin;
	return size;
}
