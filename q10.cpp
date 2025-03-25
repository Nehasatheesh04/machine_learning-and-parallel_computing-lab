#include <iostream>
#include <pthread.h>

using namespace std;

int n; // Global variable to store 'n'

// Thread function to print first n natural numbers
void* printNumbers(void* arg) {
    cout << "First " << n << " natural numbers: ";
    for (int i = 1; i <= n; i++) {
        cout << i << " ";
    }
    cout << endl;
    return NULL;
}

int main() {
    pthread_t thread;
    
    // Get user input
    cout << "Enter the value of n: ";
    cin >> n;

    // Create thread
    pthread_create(&thread, NULL, printNumbers, NULL);

    // Wait for thread to complete
    pthread_join(thread, NULL);

    cout << "Thread execution finished!" << endl;

    return 0;
}