#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>  
#include <ctime>
#include <algorithm> 

using namespace std;
using namespace chrono;

// Function to generate a random array
void generateArray(vector<int> &arr, int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Random numbers from 0 to 999
    }
}

// Sequential sum function
long long sequentialSum(const vector<int> &arr) {
    long long sum = 0;
    for (int num : arr) {
        sum += num;
    }
    return sum;
}

// Sequential search function
bool sequentialSearch(const vector<int> &arr, int key) {
    return find(arr.begin(), arr.end(), key) != arr.end();
}

// Thread function for sum calculation
void threadSum(const vector<int> &arr, int start, int end, long long &sum) {
    sum = 0;
    for (int i = start; i < end; i++) {
        sum += arr[i];
    }
}

// Thread function for search operation
void threadSearch(const vector<int> &arr, int start, int end, int key, int *found) {
    if (find(arr.begin() + start, arr.begin() + end, key) != arr.begin() + end) {
        *found = 1;
    }
}

int main() {
    int n = 1000000;  // Array size
    int key = 500;    // Element to search
    int num_threads = 4; // Number of threads
    vector<int> arr(n);
    generateArray(arr, n);

    // **SEQUENTIAL EXECUTION**
    auto start_seq = high_resolution_clock::now();
    long long sum_seq = sequentialSum(arr);
    bool found_seq = sequentialSearch(arr, key);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);

    cout << "Sequential Sum: " << sum_seq << endl;
    cout << "Key " << (found_seq ? "Found" : "Not Found") << " in Sequential Search" << endl;
    cout << "Sequential Execution Time: " << duration_seq.count() << " ms\n" << endl;

    // **MULTITHREADED EXECUTION**
    vector<thread> threads;
    vector<long long> sum_parts(num_threads, 0);
    vector<int> found_parts(num_threads, 0);  // Use int instead of bool

    int part_size = n / num_threads;
    auto start_mt = high_resolution_clock::now();

    // Launch threads for sum
    for (int i = 0; i < num_threads; i++) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? n : (i + 1) * part_size;
        threads.emplace_back(threadSum, ref(arr), start, end, ref(sum_parts[i]));
    }

    // Wait for sum threads to complete
    for (auto &t : threads) {
        t.join();
    }
    threads.clear(); // Clear thread vector

    // Compute final sum
    long long sum_mt = 0;
    for (auto &s : sum_parts) {
        sum_mt += s;
    }

    // Launch threads for search
    for (int i = 0; i < num_threads; i++) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? n : (i + 1) * part_size;
        threads.emplace_back(threadSearch, ref(arr), start, end, key, &found_parts[i]);
    }

    // Wait for search threads to complete
    for (auto &t : threads) {
        t.join();
    }

    // Determine final search result
    bool found_mt = false;
    for (int found : found_parts) {
        if (found) {
            found_mt = true;
            break;
        }
    }

    auto end_mt = high_resolution_clock::now();
    auto duration_mt = duration_cast<milliseconds>(end_mt - start_mt);

    cout << "Multithreaded Sum: " << sum_mt << endl;
    cout << "Key " << (found_mt ? "Found" : "Not Found") << " in Multithreaded Search" << endl;
    cout << "Multithreaded Execution Time: " << duration_mt.count() << " ms\n" << endl;

    return 0;
}
