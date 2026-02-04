#include <iostream>

using namespace std;

int modus(int data[], int n) {
   
    int modus = 0;
    int count = 0;
    int number = data[0];

    for (int i = 0; i < n - 1; i++)
    {
        count = 0;
        for (int j = i + 1; j < n; j++) {
            if (data[i] == data[j]) {
                count++;
            }
        }
        if (count > modus) {
            modus = count;
            number = data[i];
        }
    }
 
    return number;
}


bool isUnique(int data[], int n) {

    for (int i = 0; i < n; i++) {
    
        if (data[i] == data[i + 1])
        {
            
            return false;
        }
    
    }
    return true;
}

int partition(int data[], int low, int high) {
    int pivot = data[high];

 
    int i = low - 1;

  
    for (int j = low; j <= high - 1; j++) {
        if (data[j] < pivot) {
            i++;
            swap(data[i], data[j]);
        }
    }

  
    swap(data[i + 1], data[high]);
    return i + 1;
}

void quicksort(int data[], int low, int high) {

    if (low < high) {

        
        int pi = partition(data, low, high);

        quicksort(data, low, pi - 1);
        quicksort(data, pi + 1, high);
    }
}

int main() {

	int data[] = { 3, 1, 4, 2, 5, 3, 4, 4 };
    
    bool unique;

    int n = 8;
    quicksort(data,0,n - 1);

    isUnique(data, n - 1);

    for (int i = 0; i < n; i++)
    {
        cout << data[i] << " ";
    }

    if (!isUnique(data, n - 1))
    {
        cout << endl << "NEjedinecne";
    }
    if(isUnique(data,n-1))
    {
        cout << endl; "jedinecne";
    }

    int mod = modus(data,n);

    cout << endl << mod << endl;

	return 0;
}