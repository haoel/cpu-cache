#include <stdlib.h>
#include <algorithm> 
#include <chrono> 
#include <iomanip>
#include <iostream> 
using namespace std; 
using namespace std::chrono; 


#define LEN  ( 64 * 1024 * 1024)

// http://igoro.com/archive/gallery-of-processor-cache-effects/

void test(int* arr, int length, int steps) 
{
    // Get starting timepoint
    length--;
    auto start = high_resolution_clock::now();

    // Loop the memory
    for (int i = 0; i < steps; i++) { 
        arr[(i * 8) & length]++;
    }

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    cout << std::setw(12) << length + 1 << " : " << duration.count() << endl;
}

int main(int argc, char** argv)
{
    int length = atoi(argv[1]);
    const int steps = 64 * 1024 * 1024; 
    int* arr = new int[length];
    test(arr, length, steps);
    delete [] arr;
    
    return 0;
}
