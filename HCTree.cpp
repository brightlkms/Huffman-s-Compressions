/*
Name: Gunpod Lertkomolsuk
Date: 9th Febuary
Overview: This is an implementation file for Huffman Compression Trie.
*/
#include "HCTree.h"

using namespace std;
void HCTree::helper_del(HCNode* ptr){
	if(ptr->c0==0 && ptr->c1==0)
		delete ptr;
	else{
		helper_del(ptr->c0);
		helper_del(ptr->c1);
		delete ptr;
	}
}

HCTree::~HCTree(){
	while(pq.size()!=0){
  	helper_del(root);
  }
}
void HCTree::build(const vector<int>& freqs){
	//add all nodes to a pq
	int i=0;

	for(auto a : freqs){
		if(a!=0){
			HCNode* newNode = new HCNode(a, i);
			pq.push(newNode);
		}
		i++;
	}
	if(pq.size()==1){
		root = pq.top();
		leaves[int(pq.top()->symbol)] = root;
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
  	HCNode* newNode = new HCNode(temp1+temp2, tempN2->symbol, tempN2, tempN1);

  	tempN1->p=newNode; //higher freq
  	tempN2->p=newNode; //lower freq
  	pq.push(newNode);
  }
	//root points to the root of the trie
  root = pq.top();
  // cout << "Size (expected 1): " << pq.size() <<endl;
  // cout << "Count root node (expected: 5): "<<pq.top()->count <<endl;
  // cout << "Count left (expected: 2): "<<pq.top()->c1->count <<endl;
  // cout << "Count left (expected: C): "<<pq.top()->c1->c1->symbol <<endl;
  // cout << "Count left (expected: B): "<<pq.top()->c1->c0->symbol <<endl;
  // cout << "Count root node (expected: A): "<<pq.top()->c0->symbol <<endl;
}

void HCTree::encode(byte symbol, ofstream& out) const{	
	string binary="";
	int index=symbol;
	HCNode* sym = leaves[index];
	if (sym==root){
		binary="1"+binary;
	}
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
	char results;
	unsigned char next;

	next = in.get();
	if(in.eof()) return -1;
	
	if(root->c0==0 && root->c1==0){
		if(next == '1'){
			results=temp->symbol;
			return int(results);
		}
	}

	while(1){
		if(next == '1'){
			temp=temp->c1;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				return int(results);
			}
			next = in.get();
		}
		else if(next == '0'){
			temp=temp->c0;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				return int(results);
			}
			next = in.get();
		}
	}
	return -1;
}
