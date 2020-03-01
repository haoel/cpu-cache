#include <stdlib.h>
#include <thread>
#include <vector>
#include <algorithm> 
#include <chrono> 
#include <iomanip>
#include <iostream> 
using namespace std; 
using namespace std::chrono; 

const int repeat_num = 4*1024*1024;
const int obj_num = 32*1024;
const int pad_size = 1024;

class BadObject {
    public:
        bool isLive;
        int padding[pad_size]; // let the object > 64 Bytes
};

class GoodObject {
    public: 
        int padding[pad_size];
};


int main() 
{
    BadObject *bad = new BadObject[obj_num];
    GoodObject *good = new GoodObject[obj_num];
    bool *isLive = new bool [obj_num];

    //init objects;
    srand((unsigned int)time(NULL));
    for(int i=0; i<obj_num; i++) {
        int r = rand();
        bad[i].isLive = isLive[i] = r & 1;
        for(int j=0; j<pad_size; j++) {
            bad[i].padding[j] = good[i].padding[j] = r + j;
        }
    }

   

    auto start = high_resolution_clock::now();

    // Loop the memory
    for (int i = 0; i < repeat_num; i++) { 
        int idx = i % obj_num;
        if (bad[ idx].isLive) {
            for (int j=0; j<pad_size; j++) {
                bad[idx].padding[j] += j;
            }
        }
    }
    // Get ending timepoint
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Bad Objects  : " << duration.count() << endl;


    start = high_resolution_clock::now();

    // Loop the memory
    for (int i = 0; i < repeat_num; i++) { 
        int idx = i % obj_num;
        if (isLive[idx]) {
            for (int j=0; j<pad_size; j++) {
                good[idx].padding[j] += j;
            }
        }
    }
    // Get ending timepoint
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Good Objects : " << duration.count() << endl;

    delete [] bad;
    delete [] good;
    delete [] isLive;

}



