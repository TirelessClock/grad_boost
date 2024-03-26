#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std; 

class Loss {
    private: 

    string which;
    
    double meanSquaredError(double actual, double predicted) {
        return pow(actual - predicted, 2);
    }

    double meanAbsoluteError(double actual, double predicted) {
        return abs(actual - predicted);
    }

    double binaryCrossEntropy(double actual, double predicted) {
        const double epsilon = 1e-15;
        predicted = max(min(predicted, 1.0 - epsilon), epsilon);
        return -(actual * log(predicted) + (1.0 - actual) * log(1.0 - predicted));
    }

    double categoricalCrossEntropy(double actual, double predicted) {
        const double epsilon = 1e-15;
        predicted = max(predicted, epsilon);
        return -actual * log(predicted);
    }

    public: 

    Loss(string which) {
        this->which = which; 
    }

    double lossFunction(double actual, double predicted) {
        if(which == "MSE") {
            return meanSquaredError(actual, predicted);
        }

        else if(which == "MAE") {
            return meanAbsoluteError(actual, predicted);
        }

        else if(which == "BCE") {
            return binaryCrossEntropy(actual, predicted);
        }

        else if(which == "CCE") {
            return categoricalCrossEntropy(actual, predicted);
        }

        return -1;
    }

};