#include <bits/stdc++.h>
using namespace std;

/*
    Selection Sort
    --------------
    Time Complexity:
        Best Case   : O(N²)
        Average Case: O(N²)
        Worst Case  : O(N²)

    Space Complexity:
        O(1)

    Stable:
        No

    Idea:
    - Divide the array into two parts:
        1. Sorted part
        2. Unsorted part
    - During each pass, find the smallest element in the
      unsorted portion of the array.
    - Swap it with the first element of the unsorted part.
    - Repeat until the entire array becomes sorted.
*/

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    // Iterate through each position where the next minimum
    // element should be placed.
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        // Find the actual minimum element in the remaining array
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Place the minimum element at its correct position
        swap(arr[i], arr[minIndex]);
    }
}

// Utility function to print the array
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr;
    int num;
    while (cin >> num) {
        arr.push_back(num);
    }
    selectionSort(arr);
    printArray(arr);
    return 0;
}