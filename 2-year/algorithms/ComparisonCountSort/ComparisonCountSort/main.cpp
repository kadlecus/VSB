#include <iostream>
#include <vector>
using namespace std;

vector<int> ComparisonCountingSort(const vector<int>& A) {
    int n = A.size();
    vector<int> Count(n, 0);
    vector<int> S(n);

    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (A[i] < A[j]) {
                Count[j]++;
            }
            else {
                Count[i]++;
            }
        }
    }

    
    for (int i = 0; i < n; i++) {
        S[Count[i]] = A[i];
    }

    return S;
}

int main() {
    int n, value;
    
    cin >> n;

    vector<int> A = {};
    
    while (cin >> value) {

        A.push_back(value);
   }

    vector<int> sortedArray = ComparisonCountingSort(A);

   
    for (int i = 0; i < sortedArray.size(); i++) {
        cout << sortedArray[i] << " ";
    }
    cout << endl;

    return 0;
}