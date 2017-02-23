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

	int size = file_size(infile)*8; 
	bis.setTbit(size);

	hct.set_root();
	int leafs = binary(bis);

	traverse(hct.return_root(), bis, in, leafs);
	//--------------------
	ofstream out;
	out.open(outfile);
	
	//DECODE PADDING
	int bitshift = binary(bis);
	bis.setRemainder(bitshift);
	int r;
	unsigned char ch;
	// cout << endl;
	
	//DECODE CH
	while(1){
		r = hct.decode(bis);
		ch = r;
		// cout << r << endl;
		if (r == -1){
			// cout << "hello" << endl;
			break;
		}
		else{
			out.put(ch);
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