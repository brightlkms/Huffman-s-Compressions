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
 //  	helper_del(root);
 //  }
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
// void HCTree::build_structure(ifstream& in, BitInputStream& bis){
// 	HCNode* temp = root;
// 	struc = false;

// 	while(1){
// 		unsigned char instream = in.get();
// 		bool tb=false;

// 		// int leftc = bis.readBit();
// 		// int rightc= bis.readBit();

// 		if(root==0){			
// 			HCNode* newNode = new HCNode(1, instream);
// 			root = newNode;
// 			tb=true;
// 			temp = root;
// 			//to indicate that next byte will be twobit condition
// 		}

// 		while(tb){
// 			int leftc = bis.readBit();
// 			int rightc= bis.readBit();

// 			if(leftc==0 && rightc==0){
// 				temp = structure_helper(temp, "00", in);
// 			}
// 			if(leftc==0 && rightc==128){				
// 				temp = structure_helper(temp, "01", in);
// 			}
// 			if(leftc==128 && rightc==0){
// 				temp = structure_helper(temp, "10", in);
// 			}
// 			if(leftc==128 && rightc==128){
// 				//this is the number till base
// 				temp = structure_helper(temp, "11", in);
// 			}		
// 		}
// 	break;
// 	}
// }
// HCNode* HCTree::structure_helper(HCNode* temp, string com, ifstream& in){
// 	if(com == "00"){
// 		HCNode* newNode = new HCNode(1, temp->symbol, 0, 0, temp);
// 		temp->c1 = newNode;

// 		unsigned char instream = in.get();
// 		newNode = new HCNode(1, instream, 0, 0, temp);
// 		temp->c0 = newNode;

// 		return temp;
// 	}
// 	if(com == "01"){
// 		HCNode* newOne = new HCNode(1, temp->symbol, 0, 0, temp);
// 		temp->c1 = newOne;

// 		unsigned char instream = in.get();
// 		HCNode* newTwo= new HCNode(1, instream, 0, 0, temp);
// 		temp->c0 = newTwo;
// 		return newTwo;
// 		//continue by going back to original function you need to read the next asci
// 	}
// 	if(com == "10"){
// 		unsigned char nextCh = in.get();
// 		HCNode* newNode = new HCNode(1, nextCh, 0, 0, temp);
// 		temp->c0 = newNode;

// 		//this instream is amount till base
// 		nextCh = in.get();
// 		int c = nextCh;
// 		if(c == 2){
// 			struc = true;
// 			int leftc = bis.readBit();
// 			int rightc= bis.readBit();
// 			//Since it's the last one we wanna add to the left tree
// 			HCNode* newNode = new HCNode(1, temp->symbol, 0, 0, temp);
// 			temp->c1 = newNode;
// 			//Get last byte, do that shit
// 			nextCh = in.get();
// 			HCNode* newNode = new HCNode(1, nextCh, 0, 0, temp);
// 			temp->c0 = newNode;
// 			return temp->c1;
// 		}
// 		else{
// 			int leftc = bis.readBit();
// 			int rightc= bis.readBit();
// 			if(leftc==0 && rightc==0){
// 				temp = structure_helper(temp, "00", in);
// 			}
// 			if(leftc==0 && rightc==128){				
// 				temp = structure_helper(temp, "01", in);
// 			}
// 			if(leftc==128 && rightc==0){
// 				temp = structure_helper(temp->c0, "10", in);
// 			}
// 			if(leftc==128 && rightc==128){
// 				temp = structure_helper(temp->c1, "10", in);
// 				temp = structure_helper(temp->c0, "10", in);
// 			}
// 			return temp;
// 		}
// 	}
// 	if(com == "11"){
// 		//number
// 		instream = in.get();

// 		HCNode* newNode = new HCNode(1, temp->symbol, 0, 0, temp);
// 		temp->c1 = newNode;
// 		temp=newNode;
// 		if(c == 2){
// 			struc = true;
// 			leftc = bis.readBit();
// 			rightc= bis.readBit();
// 			//Since it's the last one we wanna add to the left tree
// 			HCNode* newNode = new HCNode(1, temp->symbol, 0, 0, temp);
// 			temp->c1 = newNode;
// 			//Get last byte, do that shit
// 			nextCh = in.get();
// 			HCNode* newNode = new HCNode(1, nextCh, 0, 0, temp);
// 			temp->c0 = newNode;
// 			return temp;
// 		}
// 		else{
// 			if(leftc==0 && rightc==0){
// 				temp = structure_helper(temp, "00", in);
// 			}
// 			if(leftc==0 && rightc==128){				
// 				temp = structure_helper(temp, "01", in);
// 			}
// 			if(leftc==128 && rightc==0){
// 				temp = structure_helper(temp->c0, "10", in);
// 			}
// 			if(leftc==128 && rightc==128){
// 			}
// 			return temp;
// 		}
// 	}
// }
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
	// vector<string>::reverse_iterator rit = per_char.rbegin();
  // for (; rit!= per_char.rend(); ++rit){
  // 	string newString = *rit;
  // 	result_in = result_in + newString;
  // }
  //indicate total bits in file
  // totalbits = result_in.length();
  // int temp = 8-result_in.length();
  // totalbits=result_in.length();
}
void HCTree::encode(byte symbol, BitOutputStream& out) const{
	int index = symbol;	
	HCNode* sym = leaves[index];
	if (sym==root){
		out.writeBit(1);
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
  	out.writeBit(*rit);
  }
}

int HCTree::decode(BitInputStream& in){
	HCNode* temp = root;
	unsigned char next;
	int results;

	while(1){
		int read = in.readBit();
		totalbits--;
		// cout << in.readBit() <<endl;
		// cout << "check read: " <<read <<endl;
		if(read==128){
			// cout << "first real bit " << endl;
			temp=temp->c1;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				// cout << "char Is :" << results <<endl;
				return results;
			}
		}
		else if(read==0){
			// cout << "should come second" << endl;
			temp=temp->c0;
			if(temp->c0==0 && temp->c1==0){
				results=temp->symbol;
				// cout << "char Is :" << results <<endl;
				return results;
			}
		}
		else if(read == -1){
			return -1;
		}
	}
}
void HCTree::set_root(){
	HCNode* newNode = new HCNode(1, 0);
	root = newNode;
}
