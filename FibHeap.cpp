#include <iostream>
#include "CircularDynamicArray.cpp"

using namespace std;

template<typename keytype>

class FibHeapNode{
    private:
        keytype value;
        FibHeapNode* parent;
        FibHeapNode* childHeadList;
        FibHeapNode* leftSibling;
        FibHeapNode* rightSibling;
        int rank;
        bool marked;
    public:
        FibHeapNode();

};



class FibHeap{
    private: 
        FibHeapNode* min;
        FibHeapNode* headRootList;
    public:
        FibHeap(){

        }
        FibHeap(keytype k[], int s, CircularDynamicArray <FibHeapNode<keytype> *> &handle);
        ~FibHeap();
        keytype peekKey(){
            return min;
        }
        keytype extractMin();
        FibHeapNode* insert(keytype k){

        }
        bool decreaseKey(FibHeapNode <keytype> *h, keytype k);
        void merge(FibHeap<keytype> &H2);
        void printKey();


};
