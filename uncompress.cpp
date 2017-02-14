#include "HCNode.cpp"
#include "HCTree.cpp"
#include <typeinfo>

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

	out << hct.decode(in);

	in.close();
	out.close();

	return 0;
}