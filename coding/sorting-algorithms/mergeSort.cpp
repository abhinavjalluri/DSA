// T.C : O(N*LOG(N))
// DIVIDE AND MERGE
#include<bits/stdc++.h>
using namespace std;

// Merges two sorted subarrays arr[i..mid] and arr[mid+1..j]
void merge(vector<int>&arr,int i,int mid,int j){
    int left = i;
    int right = mid+1;
    vector<int>temp;
    // Merge the two subarrays into temp
    while(left <= mid && right <= j){
        if(arr[left] < arr[right]){
            temp.push_back(arr[left]);
            left++;
        }
        else{
            temp.push_back(arr[right]);
            right++;
        }
    }
    // Copy any remaining elements from the left subarray
    while(left <= mid){
        temp.push_back(arr[left]);
        left++;
    }
    // Copy any remaining elements from the right subarray
    while(right <= j){
        temp.push_back(arr[right]);
        right++;
    }
    int x = 0;
    // Copy the merged elements back into the original array
    for(int index = i;index <= j;index++){
        arr[index] = temp[x];
        x++;
    }
}
// i = starting index, j = ending index
void mergesort(vector<int>&arr,int i,int j){
    if(i == j)
    return;
    int mid = (i+j)/2;
    mergesort(arr,i,mid);
    mergesort(arr,mid+1,j);
    merge(arr,i,mid,j);
}

int main() {
    int num;
    vector<int>arr;
    while(cin>>num){
        arr.push_back(num);
    }
    int n = arr.size();
    mergesort(arr,0,n-1);
    for(int i = 0;i<arr.size();i++){
        cout<<arr[i]<<" ";
    }
    return 0;
}