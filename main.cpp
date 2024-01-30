#include "DecisionTree.h"
#include<iostream>
#include<vector>
using namespace std; 

int main() {

    vector<vector<double>> data = 
    {
        {4, 6, 9, 0},
        {8, 9.9, 4, 1},
        {10, 14, 8, 0},
        {3, 2, 3, 1},
    };

    DecisionTree dt(data, 5, 3); 
    cout<<"Result: "<<dt.predict({7,9,3})<<endl;
}