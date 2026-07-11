#include <bits/stdc++.h>
using namespace std;

/*
    Insertion Sort
    --------------
    Time Complexity:
        Best Case   : O(N)      (Already sorted)
        Average Case: O(N²)
        Worst Case  : O(N²)

    Space Complexity:
        O(1)

    Stable:
        Yes

    Adaptive:
        Yes

    Idea:
    - Consider the first element as already sorted.
    - Pick the next element from the unsorted part.
    - Insert it into its correct position in the sorted part.
    - Repeat until the entire array becomes sorted.
*/

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    // Start from the second element
    for (int i = 1; i < n; i++) {
        int j = i;
        // Move the current element left until
        // it reaches its correct sorted position
        while (j > 0 && arr[j] < arr[j - 1]) {
            swap(arr[j], arr[j - 1]);
            j--;
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
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    insertionSort(arr);
    printArray(arr);
    return 0;
}