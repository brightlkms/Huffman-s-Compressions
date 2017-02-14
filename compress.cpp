#include "HCNode.cpp"
#include "HCTree.cpp"

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

	while(1){
		next = in.get();
		c =next;
		freqs[c]++;
		if(in.eof()) break;
	}
	// in.close();

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
		hct.encode(next, out);
		if(in.eof()) break;
	}
	//missing code from reading byte and encoding it to out file
	out.close();
	in.close();

	return 0;
}