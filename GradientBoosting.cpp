#include "DecisionTree.h"
#include <iostream>
#include <vector>

using namespace std; 

class GradientBoosting {

    int numberModels;
    vector<DecisionTree> models;
    double finalPrediction; 
    vector<vector<double>> dataset;
    vector<double> result;  

    void trainModels(int depthTree) {
        
        vector<double> res = result;

        for(int model = 0; model < numberModels; model++) {
            
            DecisionTree dt(dataset, res, depthTree);
            models[model] = dt; 

            for(int i=0; i<res.size(); i++) {
                res[i] = res[i] - dt.predict(dataset[i]);
            }
        }
    }

    double makePredictionBackend(vector<double> &newData) {
        double prediction = 0;
        for(auto model: models) {
            prediction += model.predict(newData);
        }
        return prediction;
    }

    public:

    GradientBoosting(vector<vector<double>> data, int numberModels, int depthTree, int yIndex) {

        cout<<"Gradient Boosting model created"<<endl;

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
        dataset = data;
        trainModels(depthTree);
    }
};