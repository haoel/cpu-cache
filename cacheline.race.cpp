#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
 
using namespace std;
using namespace chrono;
 
const int CACHE_LINE_SIZE = 64;
const int SIZE = CACHE_LINE_SIZE / sizeof(int);
const int COUNT = (10*1024*1024);
 
int main(int argc, char** argv)
{
    bool race = true;
    if (argc>1 ) race = false;

    srand((unsigned int)time(NULL));

    int* p = new int [2*SIZE];

    int *p1 = &p[0];
    int *p2 = race ? &p[1] : &p[SIZE];
 
    auto proc = [](int* data) {
        for(int i = 0; i < COUNT; ++i)
            *data += rand();
    };
    
    auto start_time = high_resolution_clock::now();
    
    std::thread t1(proc, p1);
    std::thread t2(proc, p2);
    
    t1.join();
    t2.join();
    auto end_time = high_resolution_clock::now();
    
    cout << "Duration: " << duration_cast<microseconds>(end_time - start_time).count() / 1000.f << " ms" << endl;
 
    return 0;
}
