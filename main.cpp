#include "GradientBoosting.hpp"
#include "DecisionTree.hpp"
#include "GetData.hpp"
#include <iostream>
#include <vector>
using namespace std; 

void display(vector<vector<double>> &data) {
    for (const auto& row : data) {
        for (const auto& cell : row) {
            cout << cell << "\t";
        }
        cout << "\n";
    }
}

int main() {

    vector<vector<double>> data = preprocess("data.csv", true);

    GradientBoosting gb(data, 4, 5, 5, "MAE");

    cout<<"Result: "<<gb.predict({6.8,3.0,5.5,2.1})<<"\n";

    return 0;
}

