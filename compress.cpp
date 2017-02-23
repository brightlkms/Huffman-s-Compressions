#include "HCTree.h"
#include "HCNode.h"
#include <math.h>

using namespace std;
void traverse(HCNode*, BitOutputStream&, ofstream&);
int get_twobit(HCNode* ptr, BitOutputStream&);
int find_depth(HCNode* ptr);
void binary(int task, BitOutputStream& out);
unsigned int file_size(string filename);

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
	//Find future totalbit, which is header+ch
	in.clear();
	in.seekg(0, ios::beg);
	if(leafs > 1)
	binary(leafs, bos);

	//ENCODE MAGICAL IMPOSSIBLE HEADER
	//-----------------------------------------------------
	traverse(hct.return_root(), bos, out);
	//-----------------------------------------------------
	hct.totalbits=0;
	while(1){
		next = in.get();
		if(in.eof()) break;
		hct.find_future(next);
	}
	unsigned int headerSize = file_size(outfile);
	int real_bitshift = 8-((hct.totalbits+bos.returnNbits())%8);
	//FUNCTION
	//setRemain bit takes in totalbit to find bitshift and remainBit
	//bitshift is put before encoding characters
	//remainBit is needed for write function.
	//problem: you need to find length of whole thing before writing anything
	//POST: totalbits for header and characters excluding bitshift
	in.clear();
	in.seekg(0, ios::beg);

	hct.totalbits=0;
	while(1){
		next = in.get();
		if(in.eof()) break;
		hct.find_future(next);
	}
	// hct.totalbits+=extra_bit;
	//plus bitshift (gotta reduce 8)
	// cout << "total" << hct.totalbits <<endl;
	bos.setRemainBit(hct.totalbits+8, real_bitshift);
	//ENCODE PADDED BIT
	//Put number of shift at the beginning of the file
	//POST: got total bit length
	// int extra_bit = bos.returnNbits();
	binary(real_bitshift, bos);
	//-----------------------------------------------------
	//Build tree so need to clear
	in.clear();
	in.seekg(0, ios::beg);
	//ENCODE CHAR
	while(1){
		next = in.get();
		if(in.eof()) break;
		hct.encode(next, bos);
	}
	bos.writeBit(0, true);
	//-----------------------------------------------------
	//missing code from reading byte and encoding it to out file
	out.close();
	in.close();

	return 0;
}
void traverse(HCNode* ptr, BitOutputStream& bos, ofstream& out){
	//write ASCI symbol to file
	binary(ptr->symbol, bos);
	int command = get_twobit(ptr, bos);
	//command 
	//1 = 00 //2 = 01//3 = 10//4 = 11
	//write 2 bits
	if(command == 1){
		binary(ptr->c0->symbol, bos);
	}
	//write one param
	else if(command == 2){
		traverse(ptr->c0, bos, out);
	}	
	//1.right child symbol 
	//2. number of times to get to root
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