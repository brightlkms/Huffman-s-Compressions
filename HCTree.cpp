/*
Name: Gunpod Lertkomolsuk
Date: 9th Febuary
Overview: This is an implementation file for Huffman Compression Trie.
*/
#include "HCTree.h"
#include <typeinfo>
using namespace std;

HCTree::~HCTree(){
	// while(pq.size()!=0){
  	// helper_del(root);
}

void HCTree::helper_del(HCNode* ptr){
	cout << ptr <<endl;
	if(ptr->c0==0 && ptr->c1==0)
		delete ptr;
	else{
		helper_del(ptr->c0);
		helper_del(ptr->c1);
		delete ptr;
	}
}
HCNode* HCTree::return_root(){
	return root;
}
void HCTree::build(const vector<int>& freqs){
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
  	HCNode* newNode = new HCNode(temp1+temp2, tempN1->symbol, tempN2, tempN1);

  	tempN1->p=newNode; //higher freq
  	tempN2->p=newNode; //lower freq
  	pq.push(newNode);
  }
	//root points to the root of the trie
  root = pq.top();
}

void HCTree::find_future(byte symbol){	
	
	int index=symbol;
	// vector<string> per_char;
	HCNode* sym = leaves[index];
	if (sym==root){
		totalbits+=1;
	}
	//adding what the file would have been in strings
	while(sym!=root){
		if(sym->p->c1 == sym){
			totalbits+=1;
		}
		else{
			// per_char.push_back("0");
			totalbits+=1;
		}
		sym=sym->p;
	}
}
void HCTree::encode(byte symbol, BitOutputStream& out) const{
	int index = symbol;	
	HCNode* sym = leaves[index];
	if (sym==root){
		out.writeBit(1, false);
	}
	vector<int> each_ch;
	while(sym!=root){
		if(sym->p->c1 == sym){
			each_ch.push_back(1);
		}
		else{
			each_ch.push_back(0);
		}
		sym=sym->p;
	}
	vector<int>::reverse_iterator rit = each_ch.rbegin();
  for (; rit!= each_ch.rend(); ++rit){
  	out.writeBit(*rit, false);
  }
}
int HCTree::decode(BitInputStream& in){
	HCNode* temp = root;
	unsigned char next;
	int results;
	// cout << "loop" <<endl;
	while(1){
		int read = in.readBit();
		if(read == -1){
			return -1;
		}
		if(read==128){
			temp=temp->c1;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				return results;
			}
		}
		else if(read==0){
			temp=temp->c0;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				return results;
			}
		}
		else{
			return -1;
		}
	}
}
void HCTree::set_root(){
	HCNode* newNode = new HCNode(1, 0);
	root = newNode;
}
