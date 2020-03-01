#include <stdlib.h>
#include <algorithm> 
#include <chrono> 
#include <iomanip>
#include <iostream> 
using namespace std; 
using namespace std::chrono; 

int main()
{
    srand((unsigned int)time(NULL));
    
    const int row = 1024;
    const int col = 512;

    int matrix[row][col];
    for(int r = 0; r < row; ++r) {
        for(int c=0; c<col; ++c){
            matrix[r][c] = rand() % 10;
        }
    }
 
    // row travel
    int sum_row = 0;
    auto start = high_resolution_clock::now();
    for(int r=0; r<row; r++) {
        for(int c=0; c<col; c++){
            sum_row += matrix[r][c];
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "row travel : " << sum_row << " : " << duration.count() / 1000.f << "ms" << endl;

    //column travel
    int sum_col = 0;
    start = high_resolution_clock::now();
    for(int c=0; c<col; c++) {
        for(int r=0; r<row; r++){
            sum_col += matrix[r][c];
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "col travel : " << sum_col << " : " << duration.count() / 1000.f << "ms" << endl;

    return 0;
}