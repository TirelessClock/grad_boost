#include "GradientBoosting.h"
#include<iostream>
#include<vector>
using namespace std; 

int main() {

    vector<vector<double>> data = 
    {
        {0,10,100,10},
        {1,20,180,20},
        {0,50,160,20},
        {1,32,150,20},
        {1,5,80,10},
    };

    GradientBoosting gb(data, 3, 5, 3);
    cout<<"Result: "<<gb.predict({1,15,160})<<endl;
}