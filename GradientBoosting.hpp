#pragma once 

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "DecisionTree.hpp"
#include "Loss.hpp"

using namespace std; 

class GradientBoosting {

    int numberModels;
    vector<DecisionTree*> models; 
    Loss *lossClass;

    void trainModels(vector<vector<double>> &dataset, vector<double> &result, int depthTree) {
        
        vector<double> res = result;
        vector<double> losses;

        for(int model = 0; model < numberModels; model++) {
            
            DecisionTree* dt = new DecisionTree(dataset, res, depthTree);
            models[model] = dt; 
            double l = 0;

            for(int i=0; i<result.size(); i++) {
                double prediction = dt->predict(dataset[i]);
                double lossVal = lossClass->lossFunction(res[i], prediction);
                res[i] -= prediction;
                l += lossVal;
            }

            losses.push_back(l / (result.size()*1.0));
        }

        for(int l=0; l<losses.size(); l++) {
            cout << "Model #" << l+1 << " loss function: " << losses[l] << endl;
        }
    }

    public:

    GradientBoosting(vector<vector<double>> dataset, vector<double> result, int numberModels, int depthTree, string whichLoss) {

        cout<<"Gradient Boosting model created!"<<endl;

        if(whichLoss != "MSE" && whichLoss != "MAE" && whichLoss != "BCE" && whichLoss != "CCE") {
            cout<< "Invalid Loss Function" << endl;
        }

        else {
            Loss *lossTemp = new Loss(whichLoss);
            this->lossClass = lossTemp;
        }

        this->numberModels = numberModels;
        models.resize(numberModels);
        trainModels(dataset, result, depthTree);
    }

    GradientBoosting(vector<vector<double>> data, int yIndex, int numberModels, int depthTree, string whichLoss) {

        cout<<"Gradient Boosting model created!"<<endl;

        if(whichLoss != "MSE" && whichLoss != "MAE" && whichLoss != "BCE" && whichLoss != "CCE") {
            cout<< "Invalid Loss Function" << endl;
        }

        else {
            Loss *lossTemp = new Loss(whichLoss);
            this->lossClass = lossTemp;
        }

        vector<vector<double>> dataset(data.size()); 
        vector<double> result;
        for(int i=0; i<data.size(); i++) {
            for(int j=0; j<data[i].size(); j++) {
                if(j == yIndex) {
                    result.push_back(data[i][j]);
                }
                else {
                    dataset[i].push_back(data[i][j]);
                }
            }
        }

        this->numberModels = numberModels;
        models.resize(numberModels);
        trainModels(dataset, result, depthTree);
    }

    ~GradientBoosting() {}

    double predict(vector<double> newData) {
        double prediction = 0;
        for(auto model: models) {
            prediction += model->predict(newData);
        }
        return prediction;
    }
};