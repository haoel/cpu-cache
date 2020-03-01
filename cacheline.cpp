#include <stdlib.h>
#include <algorithm> 
#include <chrono> 
#include <iomanip>
#include <iostream> 
using namespace std; 
using namespace std::chrono; 


#define LEN  ( 64 * 1024 * 1024)

//http://igoro.com/archive/gallery-of-processor-cache-effects/

void test(int* arr, int step) 
{
    srand((unsigned int)time(NULL));

    // Get starting timepoint
    auto start = high_resolution_clock::now();

    // Loop the memory
    for (int i = 0; i < LEN; i += step) arr[i] *= i;

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    cout << std::setw(6) << step << " : " << duration.count() << endl;
}

int main(int argc, char** argv)
{
     
    int step = atoi(argv[1]);
    
    int* arr = new int[LEN];
    test(arr, step);
    delete [] arr;
    
    return 0;
}
