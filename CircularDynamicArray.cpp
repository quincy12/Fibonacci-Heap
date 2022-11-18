#include <iostream>
#include <random>

using namespace std;

template<typename elmtype>

class CircularDynamicArray{
    private: 
        int size; 
        int cap; 
        int frontIndex; 
        int backIndex;
        elmtype *array;

        elmtype buffer;

    public:
        CircularDynamicArray(){
            array = new elmtype[2];
            cap = 2;
            size = 0;
            frontIndex = 0;
            backIndex = 0;
        }
        CircularDynamicArray(int s){
            array = new elmtype[s];
            cap = s;
            size = s;
            frontIndex = 0;
            backIndex = 0;
        }

        //Copy Constructor: passing in an existing CDA by reference. From src to object that invoked the copy constructor
        CircularDynamicArray(const CircularDynamicArray& src){ 
            size = src.size; //implied this->size, this->capacity, etc
            cap = src.cap;
            frontIndex = src.frontIndex;
            backIndex = src.backIndex;
            buffer = src.buffer;
            array = new elmtype[cap];
            
            for (int i=0; i < size; i++){ 
                array[i] = src.array[i];
            }
        }

        //Overloaded assignment operator: returning a reference to a CDA
        CircularDynamicArray& operator=(const CircularDynamicArray& src){
            if (this == &src) return *this;

            size = src.size;
            cap = src.cap;
            frontIndex = src.frontIndex;
            backIndex = src.backIndex;
            buffer = src.buffer;
            //CDA B (from Phase1Main.cpp) is still only a CDA of cap 2, these previous 5 lines only update the "tracking" variables; CDA B only is allocated memory for 2 elements
            array = new elmtype[cap]; //neccessary to allocate more memory
            
            for (int i=0; i < size; i++){ 
                array[i] = src.array[i];
            }
            return *this;
        }

        ~CircularDynamicArray(){
            delete[] array;
        }

        //Should print a message if i is out of bounds and return a reference to value of type elmtype stored in the class for this purpose.
        elmtype& operator[](int i){ //two usages: left and right side of equal sign. Left is assigning and right is returning a value
            if (i >= size){
                cout << "Out of bounds reference" << endl;
                return array[frontIndex];
            }
            else{
                return array[(frontIndex + i) % cap];
                //return array[mathematica(i)];
            }
        }

        void addEnd(elmtype v){
            if (size == cap){
                elmtype *tempArray;
                tempArray = new elmtype[cap];

                for (int i = 0; i < size; i++){ //copying orig values into temp array
                    tempArray[i] = array[(frontIndex + i) % cap];
                }
                array = new elmtype[cap * 2];
                for (int i = 0; i < size; i++){
                    array[i] = tempArray[i];
                }

                array[size] = v;
                size++;
                cap = cap * 2;
                frontIndex = 0;
                backIndex = size - 1;
            }

            else{
                int next = (frontIndex + size) % cap;
                array[next] = v;
                size++;
                backIndex++;
            }
        }
        

        void addFront(elmtype v){
            if (size == cap){
                elmtype *tempArray;
                tempArray = new elmtype[cap];
                
                for (int i = 0; i < size; i++){ //copying orig values into temp array
                    tempArray[i] = array[(frontIndex + i) % cap];
                }
                array = new elmtype[cap * 2];
                array[0] = v;

                for (int i = 0; i < size; i++){
                    array[i+1] = tempArray[i];
                }
                size++;
                cap = cap * 2;
                frontIndex = 0;
                backIndex = size - 1;
            }
            
            else{
                int fron = (frontIndex + cap - 1) % cap;
                array[fron] = v;
                size++;
                frontIndex = fron;
            }
        }


        void delEnd(){
            size--;
           if (size == (cap / 4)){ //resizing case 
                elmtype *tempArray;
                tempArray = new elmtype[cap/2];
                for (int i = 0; i < size; i++) tempArray[i] = array[(frontIndex + i) % cap];

                array = new elmtype[cap/2];
                for (int i = 0; i < size; i++) array[i] = tempArray[i];

                cap = cap / 2;
                backIndex = (frontIndex + size) % cap;
                frontIndex = 0;
           } 
           else{
                backIndex = (frontIndex + size-1) % cap;        
           }
        }

        void delFront(){
            size--;
            if (size == cap / 4){
                elmtype *tempArray;
                tempArray = new elmtype[cap]; //allocate mem for temp array

                for (int i = 0; i < size+1; i++) tempArray[i] = array[(frontIndex + i) % cap]; //copy orig array values into temp array
                

                array = new elmtype[cap/2]; //shrink capacity by half
                for (int i = 0; i < size; i++) array[i] = tempArray[i+1]; //copy values from tempArray into new array, starting at tempArray[1]
                
                //update information
                cap = cap/2;
                frontIndex = 0;
                backIndex = (frontIndex + size-1) % cap;

            }
            else{
                frontIndex = (frontIndex + 1) % cap;
            }
        }

        int length(){
            return size;
        }


        int capacity(){
            return cap;
        }

        void clear(){
            delete[] array;
            cap = 2;
            size = 0;
            array = new elmtype[cap];
        }

        void printActual(){
            for (int i = 0; i < cap; i++){
                cout << array[i] << " ";
            }
            cout << endl;
        }


        //quickselect choosing a random pivot. pivot = A[random(low, high)]
        elmtype QuickSelect(int k){
            elmtype zeroBaseArray[size];
            elmtype temp;
            for (int i = 0; i < size; i++){
                zeroBaseArray[i] = array[(frontIndex+i)%cap];
            }
            temp = QuickHelper(zeroBaseArray, k, size);
            for (int i = 0; i < size; i++){
                array[(frontIndex+i)%cap] = zeroBaseArray[i];
            }
            return temp;
        }

        elmtype QuickHelper(elmtype inputArray[], int k, int inputSize){
            const int from = 0;
            const int to = inputSize - 1;
            random_device rand_dev;
            mt19937 generator(rand_dev()); //stack overflow sorcery for pseduorand number
            uniform_int_distribution<int> distr(from, to);
            int pivot = inputArray[distr(generator)];

            elmtype *L = new elmtype[inputSize];
            elmtype *E = new elmtype[inputSize];
            elmtype *G = new elmtype[inputSize];
            int Ltracker, Etracker, Gtracker, Lsize, Esize, Gsize; //tracker variables for sake of array indexing
            Ltracker = Etracker = Gtracker = 0;
            for (int i = 0; i < inputSize; i++){
                if (inputArray[i] < pivot){
                    L[Ltracker++] = inputArray[i];
                }
                else if(inputArray[i] == pivot){
                    E[Etracker++] = inputArray[i];
                }
                else{
                    G[Gtracker++] = inputArray[i];
                }
            }
            Lsize = Ltracker;
            Esize = Etracker;
            Gsize = Gtracker;

            if (k <= Lsize){ 
                return QuickHelper(L, k, Lsize);
            }
            else if (k <= Lsize + Esize){
                delete[] L;
                delete[] E;
                delete[] G;
                return pivot;
            }
            else{
                return QuickHelper(G, k- Lsize - Esize, Gtracker);
            }
        }


        //Quickselect using median of medians approach
        elmtype WCSelect(int k){
            return QuickSelect(k);
        }

        // elmtype WCHelper(elmtype inputArray[], int k, int inputSize){

        // }

        //Sorts the values in the array using a comparison based O(N lg N) algorithm. The sort must be stable (doesn't swap equal values). (Mergesort)
        void stableSort(){
           elmtype zeroBaseArray[size];
           for (int i = 0; i < size; i++){
                zeroBaseArray[i] = array[(frontIndex+i)%cap];
           }
            mergesort(zeroBaseArray, size);
            for (int i = 0; i < size; i++){
                array[(frontIndex+i)%cap] = zeroBaseArray[i];
            }
        }

        void mergesort(elmtype inputArray[], int n){ //n is the size of array to be sorted
            if (n <= 1)return;
            elmtype half1[n/2];
            elmtype half2[n-(n/2)];
            for (int i = 0; i < n/2; i++){
                half1[i] = inputArray[i];
            }
            for (int i = n/2; i < n; i++){
                half2[i-(n/2)] = inputArray[i];
            }
            mergesort(half1, n/2);
            mergesort(half2, n-(n/2));
            merge(inputArray, half1, half2, n);
        }

        void merge(elmtype actualArray[], elmtype inputArray1[], elmtype inputArray2[], int n){
            int size1 = n/2; //size of first input array
            int size2 = n-(n/2); //size of second input array
            int p1 = 0; //pointer for first input array
            int p2 = 0; //pointer for second input array
            int i = 0;
            while (p1 < size1 && p2 < size2){
                if (inputArray1[p1] < inputArray2[p2]){
                    actualArray[i] = inputArray1[p1];
                    p1++;
                }
                else{
                    actualArray[i] = inputArray2[p2];
                    p2++;
                }
                i++;
            }
            while (p1 < size1){
                actualArray[i] = inputArray1[p1];
                p1++;
                i++;
            }
            while (p2 < size2){
                actualArray[i] = inputArray2[p2];
                p2++;
                i++;
            }
        }

        //Performs a linear search of the array looking for the item e. Returns the index of the item if found or -1 otherwise.
        int linearSearch(elmtype e){
            for (int i = 0; i < size; i++){
                if (array[(frontIndex + i) % cap] == e) return i;
            }
            return -1;
        }

        //Performs a binary search of the array looking for the item e. Returns the index of the item if found or -1 otherwise. Assumes that the array is in sorted order.
        int binSearch(elmtype e){
            int l, r, n, middle, halfSize;
            l = frontIndex;
            r = backIndex;
            n = size;
            halfSize = n / 2;
            middle = ((halfSize) + frontIndex) % cap;
            while (n >= 1){
                if (array[middle] == e)
                {
                    return (cap + middle - frontIndex) % cap;
                }
                else if(array[middle] > e) //we need to search left half of array now
                { 
                    n = n / 2;
                    r = (middle - 1 + cap) % cap; //update right bound to be middle minus one; adding cap and mod cap handles case when middle == 0 or other bullshit
                    middle = (l + n/2) % cap;
                }
                else //(array[middle] < e); need to search right half of array now
                {
                    n = n / 2;
                    l = (middle + 1) % cap; //update left bound to be middle plus 1 mod cap; dont have to add cap before mod because adding one can't make it 0
                    middle = ((r - (n/2)) + cap) % cap;
                }
            }
            return -1; //case for when e is not found, while loop only exits without a return when it isn't found
        }  
};




        