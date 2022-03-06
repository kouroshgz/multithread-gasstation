#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <chrono>
using namespace std;
#define SECONDS 30
mutex m1;
mutex m2;
// counters representing each pump, incremented per fillup
int pump1 = 0;
int pump2 = 0;
// int vector where each index corresponds to a threads tid, counting how often a thread reaches a pump
vector<int> fillups(10, 0);

void fillup(int tid) {
    auto start = chrono::high_resolution_clock::now();
    int duration = 0.0;
    // runs for SECONDS 
    while (duration < SECONDS) {    
    // try pump1
    if (m1.try_lock()) {
        pump1++;
        fillups[tid]++;
        this_thread::sleep_for(30ms);
        m1.unlock();

    // if pump1 not available, try pump2
    } else if (m2.try_lock()) {
        pump2++;
        fillups[tid]++;
        this_thread::sleep_for(30ms);
        m2.unlock();
    }
    auto now = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::seconds>(now - start).count();      
    }
   
}

int main() {
    thread *tArr = new thread[10];
    auto start = chrono::high_resolution_clock::now();
    // launch the car threads
    for (int i = 0; i < 10; i++) {
        tArr[i] = thread(fillup, i);
    }
    // join the car threads
    for (int i = 0; i < 10; i++) {
        tArr[i].join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    double seconds = duration.count()/1000000000.0;
    cout << "run took " << seconds << " seconds " << endl;
    cout << "Pump 1 served: " << pump1 << " cars. " << endl;
    cout << "Pump 2 served: " << pump2 << " cars. " << endl;
    for (int i = 0; i < 10; i++) {
        cout << "Car " << i << " filled up " << fillups[i] << " times. " << endl;
    }
    delete[] tArr;

    return 0;
}