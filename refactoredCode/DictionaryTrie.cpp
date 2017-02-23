/*
Name: Gunpod Lertkomolsuk
Date: 9th Febuary
Overview: This file is an implementation file for Dictionary Ternary Search Tree
*/
#include "util.h"
#include "DictionaryTrie.h"

using namespace std;
/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie() : root(nullptr) {}

DictionaryTrie::TrieNode::TrieNode(const char & d) : left(0), right(0), child(0), label(d), lword(false) {}

void DictionaryTrie::addEachKey(string word, unsigned int freq, TrieNode* curr, int index)
{
  if(!root){
    root = new TrieNode(word[index]);
    curr = root;

    if(index == word.length()-1){
      curr->freq=freq;
      curr->lword=true;
    }
    index++;
  }
  if(index == word.length()){
      curr->freq=freq;
      curr->lword=true;
  }
	for(unsigned int i = index; i<word.length();i++){
		TrieNode* newNode = new TrieNode(word[i]);
		curr->child = newNode;
		curr=curr->child;
		if(i == word.length()-1){
			curr->freq=freq;
			curr->lword=true;
		}
	}
}
/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(string word, unsigned int freq)
{
	unsigned int index=0;
	char letter = word[index];
  TrieNode* curr = root;

	if(!root){
  	addEachKey(word, freq, root, index);  
		return true;
	}
  while(index!=word.length()){
    letter = word[index];
  	if(curr->label > letter){
  		if(curr->left){
  			curr=curr->left;	
  		}
  		else{
  			TrieNode* newNode = new TrieNode(word[index]);
        curr->left = newNode;
        curr=curr->left;
        index++;
				addEachKey(word, freq, curr, index);
    		return true;
      }
    }
  	else if(curr->label < letter){
  		if(curr->right){
  			curr=curr->right;
  		}
  		else{
  			TrieNode* newNode = new TrieNode(word[index]);
  			curr->right = newNode;
  			curr = curr->right;
        index++;
				addEachKey(word, freq, curr, index);
  			return true;
  		}
  	}
  	else if(letter == curr->label){
      //last ch condition
      if(index == word.length()-1){
        if(curr->lword){
          if(freq > curr->freq){
            curr->freq=freq;
          }
          else{
            curr->freq+=freq;
          }
          return false;
        }
        else{
          curr->lword=true;
          curr->freq=freq;
          return true;
        }
      }
  		else if(curr->child){
  			curr=curr->child;
  			index++;
  		}
      else{
        index++;
        addEachKey(word, freq, curr, index);
        return true;
      }
  	}
  }
  return false;
}
/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(string word) const
{
	TrieNode* curr = root;
  unsigned int index=0;
  char letter = word[index];

	if(!root){
		return false;
	}
  while(true){
  	letter = word[index];
    if(!isalpha(int(letter)) && int(letter) != 32){
      cout<< "Invalid" <<endl; 
      return false; 
    }
  	if(letter < curr->label){
  		if(curr->left){
  			curr=curr->left;	
  		}
  		else{
  			return false;
  		}
  	}
  	else if(letter > curr->label){
  		if(curr->right){
  			curr=curr->right;
  		}
  		else{
  			return false;
  		}
  	}
  	else if(letter == curr->label){
  		if(index == word.length()-1){
  			return curr->lword;
  		}
  		else if(curr->child){
  			curr=curr->child;
  			index++;
  		}
  		else{
  			return false;
  		}
  	}
  }
}
/* Returns TrieNode pointer to node where last node of targeted prefix was found*/
DictionaryTrie::TrieNode* DictionaryTrie::find_prefix(string word){
  TrieNode* curr = root;
  unsigned int index=0;
  char letter = word[index];
  if(!root){
    curr = nullptr;
    return curr;
  }
  while(true){
    letter = word[index];
    if(!isalpha(int(letter))){
      cout << "Invalid Input. Please retry with correct input" <<endl;
      curr = nullptr;
      return curr; 
    }
    if(letter < curr->label){
      if(curr->left){
        curr=curr->left;  
      }
      else{
        curr = nullptr;
        return curr;
      }
    }
    else if(letter > curr->label){
      if(curr->right){
        curr=curr->right;
      }
      else{
        curr = nullptr;
        return curr;
      }
    }
    else if(letter == curr->label){
      if(index == word.length()-1){
        return curr;
      }
      else if(curr->child){
        curr=curr->child;
        index++;
      }
      else{
        curr = nullptr;
        return curr;
      }
    }
  }
}
/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
vector<string> DictionaryTrie::predictCompletions(string prefix, unsigned int num_completions)
{
  vector<string> words;
  set<pair<int, string>> inorder;
  string newWord="";
  unsigned int count=0;

  if(prefix=="") {
    cout << "Invalid Input. Please retry with correct input" <<endl;
    return words;
  }
  else{
    TrieNode* endOfPrefix = find_prefix(prefix);//should return pointer to last word of prefix
    if(!endOfPrefix){
      return words;
    }
    else{ 
      if(endOfPrefix->lword){  
        pair<int, string> pref (endOfPrefix->freq, prefix);
        inorder.insert(pref);
      }
      predictHelper(endOfPrefix->child, inorder, prefix, newWord);
    }
    set<pair<int, string>>::reverse_iterator it=inorder.rbegin();
    for (; it!=inorder.rend(); ++it){
      if (count == num_completions)
        break;
      words.push_back((*it).second);
      count++;
    }
    return words;
  }
}
void DictionaryTrie::predictHelper(TrieNode* n, set<pair<int,string>>& inorder, string prefix, string word)
{
  if(n==nullptr){
    return;
  }
  string oldword=word;
  string newWord=word+(n->label);
  string addWord = prefix+newWord;
  if(n->lword){
    pair<int, string> newly (n->freq, addWord);
    inorder.insert(newly);
  }
  predictHelper(n->left, inorder, prefix, oldword);
  predictHelper(n->child, inorder, prefix, newWord);
  predictHelper(n->right, inorder, prefix, oldword);
}
/* Destructor */
DictionaryTrie::~DictionaryTrie(){}
