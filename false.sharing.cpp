#include <stdlib.h>
#include <thread>
#include <vector>
#include <algorithm> 
#include <chrono> 
#include <iomanip>
#include <iostream> 
using namespace std; 
using namespace std::chrono; 

const int total_size = 16*1024*1024;
int* test_data = NULL;
const int max_threads = 128;
int result[max_threads];
int nthread = 4;
int total_count = 0;
int chunk_size = 0;

int sum(int len) {
    int r = 0;
    for (int i=0; i<len; i++) {
        r += result[i];
    }
    return r;
}

void count()
{
    for(int i = 0; i < total_size; ++i) {
        if ( test_data[i] % 2 !=0 ) total_count++;
    }
}

float thread_test( function<void(int)> func ) {
    auto start_time = high_resolution_clock::now();
    vector<thread> threads;
    for (int i=0; i<nthread; i++) {
        auto t = std::thread(func, i);
        threads.emplace_back(move(t));
    }

    for (auto& t : threads) {
        t.join();
    }
    auto end_time = high_resolution_clock::now();

    count();
    int c = sum(nthread);
    if (total_count != c) {
        cout << "count = " << total_count << "," << c << endl;
    }

    return duration_cast<microseconds>(end_time - start_time).count();
}

int main(int argc, char** argv) {

    if (argc > 1) {
        nthread = min( atoi(argv[1]), max_threads);
    }
    srand((unsigned int)time(NULL));
    test_data = (int *)malloc(total_size * sizeof(int));
    for (int i=0; i<total_size; i++) {
        test_data[i] = rand();
    }
    chunk_size = total_size / nthread + 1;

    // false sharing way
    auto false_sharing_proc = [](int id) {
        result[id] = 0;
        int start = id * chunk_size;
        int end = min(start + chunk_size, total_size);

        for ( int i = start; i < end; ++i ) {
            if (test_data[i] % 2 != 0 ) ++result[id];
        }
    };
    
    // non-false sharing way
    auto non_false_sharing_proc = [](int id) {
        result[id] = 0;
        int start = id * chunk_size;
        int end = min(start + chunk_size, total_size);

        int tmp_count = 0;
        for ( int i = start; i < end; i++ ) {
            if ( test_data[i] % 2 != 0 )  ++tmp_count;
        }
        result[id] = tmp_count;
    };

    float time;
    if (argc > 2) {
        time = thread_test(false_sharing_proc);
    }else{
        time = thread_test(non_false_sharing_proc); 
    }

    cout <<  time << endl;

    return 0;
 
}