/*
Name: Gunpod Lertkomolsuk
Date: 22th Febuary
Overview: This is protocol file for Huffman Compression Node Class.
*/
#ifndef HCTREE_H
#define HCTREE_H

#include <queue>
#include <vector>
#include <fstream>
#include <string>
#include "HCNode.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;
    //note on leaves: each index is the value of the symbol youre trying to encode.
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    
public:
    // explicit keyword is used to avoid accidental implicit conversions
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
    }
    ~HCTree();

    int totalbits;
    bool struc;
    //traverse tree counting how many bits will be needed to encode the text.
    void find_future(byte symbol);
    //delete node recursively
    void helper_del(HCNode*);
    HCNode* return_root();
    //Set root of HCtree to a certain node to facilitate decode when decompressing.
    void set_root();
    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);
    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in);
};

#endif // HCTREE_H
