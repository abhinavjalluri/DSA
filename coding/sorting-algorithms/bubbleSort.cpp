#include <bits/stdc++.h>
using namespace std;

/*
    Bubble Sort
    -----------
    Time Complexity:
        Best Case   : O(N)   (Already sorted, due to optimization)
        Average Case: O(N²)
        Worst Case  : O(N²)

    Space Complexity:
        O(1)

    Idea:
    - Compare adjacent elements.
    - Swap them if they are in the wrong order.
    - After each pass, the largest unsorted element moves to its correct
      position at the end of the array.
*/

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    // Perform n-1 passes
    for (int pass = n - 1; pass >= 1; pass--) {
        bool swapped = false;
        // Compare adjacent elements
        for (int i = 0; i < pass; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        // If no swaps happened, the array is already sorted
        if (!swapped){
            break;
        }
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
    bubbleSort(arr);
    printArray(arr);
    return 0;
}