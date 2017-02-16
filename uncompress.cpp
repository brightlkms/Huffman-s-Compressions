#include "HCNode.h"
#include "HCTree.h"
#include <typeinfo>

using namespace std;
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
	//for counting occurences of bytes
	vector<int> freqs (256, 0);
	for(int i=0;i<256;i++){
		getline(in, line);
		freqs[i]=stoi(line);
	}

	HCTree hct;
	hct.build(freqs);

	ofstream out;
	out.open(outfile);

	while(1){
		char ch = hct.decode(in);
		if (ch!=-1)
			out << ch;
		else
			break;
		// if(in.eof()) break;
	}
	
	in.close();
	out.close();
}
