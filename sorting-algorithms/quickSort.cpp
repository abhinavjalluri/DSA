// ------------------------------------------------------------
// QUICK SORT
// ------------------------------------------------------------
// Algorithm      : Divide and Conquer
//
// Idea:
// 1. Choose a Pivot element.
// 2. Place the pivot in its correct sorted position.
// 3. Elements smaller than pivot go to the left.
// 4. Elements greater than pivot go to the right.
// 5. Recursively repeat the same process on both halves.
//
// Time Complexity:
// Best Case    : O(N log N)
// Average Case : O(N log N)
// Worst Case   : O(N²)
//
// Space Complexity:
// Average : O(log N) (Recursive Stack)
// Worst   : O(N)
//
// This implementation chooses the FIRST element as Pivot.
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

/*
------------------------------------------------------------
Function : partition()

Purpose:
--------
Places the pivot in its correct sorted position.

Before Partition:

    [ Smaller Elements | Pivot | Larger Elements ]
          (Not arranged)

After Partition:

    [ Smaller ] Pivot [ Larger ]

Returns:
--------
Index where pivot is finally placed.
------------------------------------------------------------
*/

int partition(vector<int> &arr, int low, int high){
    // First element is chosen as Pivot
    int pivot = arr[low];
    // Two pointers
    int i = low;
    int j = high;
    while (i < j){
        // Move i until an element greater than pivot is found
        while (i <= high - 1 && arr[i] <= pivot){
            i++;
        }
        // Move j until an element smaller than or equal to pivot is found
        while (j >= low + 1 && arr[j] > pivot){
            j--;
        }
        // Swap misplaced elements
        if (i < j){
            swap(arr[i], arr[j]);
        }
    }

    // Place pivot at its correct position
    swap(arr[low], arr[j]);
    return j;
}

/*
Function : QuickSort()

Steps:
------
1. Find Pivot Position
2. Sort Left Half
3. Sort Right Half

Base Case:
----------
If low >= high,
array has one or zero elements,
which is already sorted.
------------------------------------------------------------
*/

void QuickSort(vector<int> &arr, int low, int high){
    if (low >= high)
        return;

    // Pivot reaches its correct position
    int pivotIndex = partition(arr, low, high);
    // Sort left side
    QuickSort(arr, low, pivotIndex - 1);
    // Sort right side
    QuickSort(arr, pivotIndex + 1, high);
}

int main(){
    vector<int> arr;
    int num;
    while (cin >> num){
        arr.push_back(num);
    }
    int n = arr.size();
    if (n == 0){
        return 0;
    }
    QuickSort(arr, 0, n - 1);
    cout << "Sorted Array : ";
    for (int x : arr){
        cout << x << " ";
    }
    return 0;
}