/*
Name: Gunpod Lertkomolsuk
Date: 9th Febuary
Overview: This is an implementation file for Huffman Compression Trie.
*/
#include "HCTree.h"

using namespace std;

void HCTree::build(const vector<int>& freqs){
	priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
	//add all nodes to a pq
	int i=0;
	for(auto a : freqs){
		if(a!=0){
			HCNode* newNode = new HCNode(a, i);
			pq.push(newNode);
		}
		i++;
	}
	
  while(pq.size() != 1){
  	int temp1 = pq.top()->count;
  	HCNode* tempN1 = pq.top();
  	if(pq.top()->c0 == 0 && pq.top()->c1 == 0){
  		leaves[int(pq.top()->symbol)] = tempN1;	
  	}
  	pq.pop();
  	
  	int temp2 = pq.top()->count;
  	HCNode* tempN2 = pq.top();
  	if(pq.top()->c0 == 0 && pq.top()->c1 == 0){
  		leaves[int(pq.top()->symbol)] = tempN2;
  	}
  	pq.pop();

  	//symbol of parent will be left child the smaller one 
  	HCNode* newNode = new HCNode(temp1+temp2, 0, tempN2, tempN1);

  	tempN1->p=newNode;
  	tempN2->p=newNode;
  	pq.push(newNode);
  }
	//root points to the root of the trie
  root = pq.top();
  // cout << "Size (expected 1): " << pq.size() <<endl;
  // cout << "Count root node (expected: 4): "<<pq.top()->count <<endl;
  // cout << "Count root node (expected: B): "<<pq.top()->c1->symbol <<endl;
  // cout << "Count root node (expected: C): "<<pq.top()->c0->c1->symbol <<endl;
  // cout << "Count root node (expected: A): "<<pq.top()->c0->c0->symbol <<endl;
}

void HCTree::encode(byte symbol, ofstream& out) const{
	int getAsci = int(symbol);
	
	string binary="";
	int index=symbol;
	HCNode* sym = leaves[index];

	while(sym!=root){
		if(sym->p->c1 == sym){
			binary="1"+binary;
		}
		else{
			binary="0"+binary;
		}
		sym=sym->p;
	}
	out << binary;
}

int HCTree::decode(ifstream& in) const{
	HCNode* temp = root;
	string results= "";
	unsigned char next;
	while(1){
		next = in.get();
		if(next == '1'){
			temp=temp->c1;
		}
		else if(next == '0'){
			temp=temp->c0;
		}
		if(temp->c0==0 && temp->c1==0){
			results+=temp->symbol;
			temp=root;
		}
		if(in.eof()) break;
	}
	return 0;
}
