#include "HCTree.h"
#include "HCNode.h"

using namespace std;
int main(int argc, char** argv){
	cout << "Compression Running" <<endl;
	if(argc < 2){
		cout << "Compress requires 2 arguments infile and outfile" <<endl;
		return -1;
	}
	string infile = argv[1];
	string outfile = argv[2];

	ifstream in;
	in.open(infile);
	unsigned char next;
	int c;
	//for counting occurences of bytes
	vector<int> freqs (256, 0);

	if(in.peek() == ifstream::traits_type::eof()){
		cout<<"File is empty";
		return -1;
	}
	while(1){
		next = in.get();
		if(in.eof()) break;
		else{
			c=next;
			freqs[c]++;
		}
	}
	//Constructing Huff man
	HCTree hct;
	hct.build(freqs);
	
	ofstream out;
	out.open(outfile);
	//writing header file (need to fix this)
	for(auto a : freqs){
		out << a << endl;
	}
	in.clear();
	in.seekg(0, ios::beg);
	while(1){
		next = in.get();
		if(in.eof()) break;
		hct.encode(next, out);
	}
	//missing code from reading byte and encoding it to out file
	out.close();
	in.close();

	return 0;
}
