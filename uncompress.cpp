#include "HCNode.h"
#include "HCTree.h"
#include <typeinfo>

using namespace std;

unsigned int file_size(string filename);

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

	string line;
	//--------------------
	//DECODE HEADER
	vector<int> freqs (256, 0);
	for(int i=0;i<256;i++){
		getline(in, line);
		freqs[i]=stoi(line);
	}

	HCTree hct;
	hct.build(freqs);

	//--------------------
	ofstream out;
	out.open(outfile);
	BitInputStream bis(in);
	//DECODE PADDING
	next = in.get();
	int bitshift = next;
	int size = file_size(infile);
	bis.trackRemainBit(size, bitshift);
	//hard cod total but for tree need to come back and fix this
	hct.totalbits = (((size-512)-1)*8);
	
	//DECODE CH
	while(1){
		char ch = hct.decode(bis);
		if (ch != -1){
			out << ch;
		}
		else
			break;
	}
	
	in.close();
	out.close();
}

unsigned int file_size(string filename){
	streampos begin,end;
	ifstream myfile (filename, ios::binary);
	begin = myfile.tellg();
	myfile.seekg (0, ios::end);
	end = myfile.tellg();
	myfile.close();
	// cout << "size is: " << (end-begin) << " bytes.\n";
	unsigned int size = end-begin;
	return size;
}
