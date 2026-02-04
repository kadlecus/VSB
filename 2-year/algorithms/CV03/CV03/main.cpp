#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;


bool CountMatrix(vector<vector<double>>& matrix, vector<double>& vector) {
    int n = matrix.size();

    
    for (int i = 0; i < n; ++i) {
        matrix[i].push_back(vector[i]);
    }

    
    for (int i = 0; i < n - 1; ++i) {
        int pivotrow = i;

        
        for (int j = i + 1; j < n; ++j) {
            if (abs(matrix[j][i]) > abs(matrix[pivotrow][i])) {
                pivotrow = j;
            }
        }

       
        for (int k = i; k <= n; ++k) {
            swap(matrix[i][k], matrix[pivotrow][k]);
        }

        
        for (int j = i + 1; j < n; ++j) {
            double temp = matrix[j][i] / matrix[i][i];
            for (int k = i; k <= n; ++k) {
                matrix[j][k] = matrix[j][k] - matrix[i][k] * temp;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        bool allZeroA = true;
        for (int j = 0; j < n; ++j) {
            if (abs(matrix[i][j]) > 1e-9) {
                allZeroA = false;
                break;
            }
        }
        if (abs(matrix[i][n]) < 1e-9) {


            return false;
        }
    }

     vector[n - 1] = matrix[n - 1][n] / matrix[n - 1][n - 1];


     for (int i = n - 2; i >= 0; --i) {
         double suma = matrix [i][n];
         for (int j = i + 1; j < n; ++j) {
             suma -= matrix[i][j] * vector[j];
         }
         vector[i] = suma / matrix[i][i];
     }
     return true;
}


vector<vector<double>> readMatrix() {
    vector<vector<double>> matrix;
    string line;

    while (getline(cin, line) && !line.empty()) {
        vector<double> row;
        istringstream iss(line);
        double value;
        while (iss >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    return matrix;
}

vector<double> readVector() {
    vector<double> vec;
    string line;

    getline(cin, line);
    istringstream iss(line);
    double value;
    while (iss >> value) {
        vec.push_back(value);
    }

    return vec;
}

void printMatrix(const vector<vector<double>>& matrix) {
    cout << "Matrix:" << endl;
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            cout << setw(10) << fixed << setprecision(3) << elem << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void printVector(const vector<double>& vec) {
    cout << "Vector:" << endl;
    for (const auto& elem : vec) {
        cout << setw(10) << fixed << setprecision(3) << elem << " ";
    }
    cout << endl << endl;
}

int main() {
    vector<vector<double>> matrix = readMatrix();
    vector<double> vec = readVector();

    if (!CountMatrix(matrix, vec)) {
        cout << "Nekonecne mnoho rerseni\n";
    }
    else
    {
        printVector(vec);

    }
   
    return 0;
}