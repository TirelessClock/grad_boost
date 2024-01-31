#include "DecisionTree.h"
#include<iostream>
#include<vector>
using namespace std; 

int main() {

    vector<vector<double>> data = 
    {
        {0,10,100},
        {1,20,180},
        {0,50,160},
        {1,32,150},
        {1,5,80},
    };

    DecisionTree dt(data, {10,20,20,20,10}, 5); 
    dt.printTree();
    cout<<"Result: "<<dt.predict({1,15,160})<<endl;
}