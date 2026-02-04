#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> heap;

    void heapify(int i) {
        // TODO: Implementujte metodu heapify

        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < heap.size() && heap[l] > heap[largest]) {
            largest = l;
        }
        if (r < heap.size() && heap[r] > heap[largest])
        {
            largest = r;
        }

        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapify(largest);
        }
    }

public:
    void insert(int key) {
        // TODO: Implementujte metodu pro vložení prvku

        heap.push_back(key);
       
        int i = heap.size() - 1;

        
        while (i > 0 && heap[(i - 1) / 2] < heap[i]) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }

    }

    int extractMax() {
        // TODO: Implementujte metodu pro odebrání a vrácení nejvìtšího prvku
        int root = heap[0];

        
        heap[0] = heap.back();
        heap.pop_back();

        
        heapify(0);

        return root;
    }

    void printHeap() {
        for (int i : heap) {
            cout << i << " ";
        }
        cout << endl;
    }

    bool isEmpty() {
        return heap.empty();
    }
};

int main() {
    MaxHeap maxHeap;
    int num;

    // Naètení èísel ze stdin
    while (cin >> num) {
        maxHeap.insert(num);
    }

    // Postupné odebírání prvkù a tisk haldy
    while (!maxHeap.isEmpty()) {
        cout << "(" << maxHeap.extractMax() << ") ";
        maxHeap.printHeap();
    }

    return 0;
}