#include <chrono>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

const int repeat_times = 10000000;

void test_memory(vector<int>& memory, int increment)
{
    using Clock = std::chrono::steady_clock;

    auto size = memory.size();
    auto start = Clock::now();

    for (int i = 0; i < repeat_times; i++) {
        for (int j = 0; j < size; j += increment) {
            memory[j] += j;
        }
    }

    cout << duration_cast<microseconds>(Clock::now() - start).count() << endl;
}

int main(int argc, char** argv)
{

    auto count = static_cast<size_t>(std::stoi(argv[1]));
    auto increment = static_cast<size_t>(std::stoi(argv[2])) / sizeof(int);

    vector<int> memory(count * increment);
    test_memory(memory, increment);

    return 0;
}