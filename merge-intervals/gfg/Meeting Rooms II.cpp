// Given two arrays start[] and end[] such that start[i] is the starting time of ith meeting and end[i] is the ending time of ith meeting. Return the minimum number of rooms required to attend all meetings.

// Note: A person can also attend a meeting if it's starting time is same as the previous meeting's ending time.

// Examples:

// Input: start[] = [1, 10, 7], end[] = [4, 15, 10]
// Output: 1
// Explanation: Since all the meetings are held at different times, it is possible to attend all the meetings in a single room.
// Input: start[] = [2, 9, 6], end[] = [4, 12, 10]
// Output: 2
// Explanation: 1st and 2nd meetings at one room but for 3rd meeting one another room required.
// Constraints:
// 1 ≤ start.size() = end.size() ≤ 105
// 0 ≤ start[i] < end[i] ≤ 106

class Solution {
  public:
    int minMeetingRooms(vector<int> &start, vector<int> &end) {
        vector<vector<int>>meetings;
        int n = start.size();
        for(int i = 0 ; i < n ; i++){
            meetings.push_back({start[i], end[i]});
        }
        sort(meetings.begin(), meetings.end());
        // Use a min-heap (priority queue) to keep track of the end times of meetings currently using a room
        priority_queue<int, vector<int>, greater<int>> pq;
        pq.push(meetings[0][1]);
        int rooms = 1;
        for(int i = 1 ; i < n ; i ++){
            // If the current meeting starts before the earliest ending meeting ends, we need a new room
            if(meetings[i][0] < pq.top()){
                rooms++;
            }
            // If the current meeting starts after or when the earliest ending meeting ends, we can reuse that room
            else{
                pq.pop();
            }
            pq.push(meetings[i][1]);
        }
        return rooms;
    }
};
