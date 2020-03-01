#include <chrono>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

const int repeat_times = 1000;


void test_memory(vector<int>& memory)
{
    using Clock = std::chrono::steady_clock;

    size_t size = memory.size();

    int value = rand();
    auto start = Clock::now();

    for (int i = 0; i < repeat_times; i++) {
        for (int j = 0; j < size; j++) {
            memory[j] = value;
        }
    }

    auto time = duration_cast<microseconds>(Clock::now() - start).count();
    auto timePerIter = time / (double) repeat_times;
    auto bytes = size * sizeof(int);

    cout << bytes / timePerIter << endl;  // MiB/s
}

int main(int argc, char** argv)
{
    long long size = 0;
    if (argc > 1){
        size = atoi(argv[1]);
    }
    size_t count = size / sizeof(int);
    vector<int> data(count, 0);

    test_memory(data);

    return 0;
}