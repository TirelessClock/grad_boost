#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

double giniIndex(vector<double> &v) {
    double sumDistribution = (double)(v.size());

    unordered_map<double, double> classDistribution;
    for(double i=0; i<(double)v.size(); i++) {
        classDistribution[v[i]]++;
    }

    double gI = 1.0;
    for(auto it: classDistribution) {
        gI -= (it.second / sumDistribution) * (it.second / sumDistribution);
    }

    return gI;
}

class Node {

    vector<vector<double>> dataset; 
    vector<double> result;
    
    double allowedDepth;
    double numRows;
    double numColumns;
    double giniImpurity;

    public:

    Node* left; 
    Node* right; 
    int colToConsider; 
    double threshold;

    Node(vector<vector<double>> dataset, vector<double> result, double k, double giniImpurity) {
        this->dataset = dataset; 
        this->result = result; 
        
        left = nullptr;
        right = nullptr;
        
        allowedDepth = k;
        numRows = dataset.size();
        numColumns = dataset[0].size();
        this->giniImpurity = giniImpurity;

    }

    bool isLeaf() {
        return (allowedDepth == 0 or numRows <= 1);
    }

    void determineSplit() {

        if(isLeaf()) return;

        if(giniImpurity == -1) {
            giniImpurity = giniIndex(result);
        }

        double giniGain = -1;

        for(int col=0; col<(int)numColumns; col++) {

            vector<double> temp(numRows); 
            for(int row=0; row<(int)numRows; row++) {
                temp[row] = dataset[row][col];
            }

            sort(temp.begin(), temp.end());

            double mid = numRows/2 - 1;
            while(mid < numRows-1 and temp[mid] == temp[mid+1]) {mid++;} 

            double pivot = temp[mid];
 
            vector<vector<double>> dataLeft, dataRight;
            vector<double> resultLeft, resultRight;

            for(int row=0; row<(int)numRows; row++) {
                if(dataset[row][col] <= pivot) {
                    dataLeft.push_back(dataset[row]);
                    resultLeft.push_back(result[row]);
                }
                else {
                    dataRight.push_back(dataset[row]);
                    resultRight.push_back(result[row]);
                }
            }

            double gILeft = giniIndex(resultLeft);
            double gIRight = giniIndex(resultRight);

            double sizeLeft = (double)resultLeft.size();
            double sizeRight = (double)resultRight.size();

            double weightedAverage = ((sizeLeft / numRows)*(gILeft)) + ((sizeRight / numRows)*(gIRight));

            double gG = giniImpurity - weightedAverage;

            if(gG > giniGain) {
                giniGain = gG; 
                if(dataLeft.size() > 0) left = new Node(dataLeft, resultLeft, allowedDepth-1, gILeft);
                if(dataRight.size() > 0)right = new Node(dataRight, resultRight, allowedDepth-1, gIRight);
                
                colToConsider = col;
                threshold = pivot;
            }

        }
    }

    double returnResults() {
        unordered_map<double, double> freq; 

        for(int i=0; i<(int)numRows; i++) {
            freq[result[i]]++;
        }

        double res;
        double freqMax = -1;
        for(auto it: freq) {
            if(it.second > freqMax) {
                freqMax = it.second; 
                res = it.first; 
            }
        }

        return res; 

    }

};

class DecisionTree {

    Node* root; 
    
    void treeSplit(Node* root) {
        
        if(!root) return;
        
        root->determineSplit();
        
        treeSplit(root->left);
        treeSplit(root->right);
    }

    double predictionBackend(Node* node, vector<double>& newData) {
        if(node->isLeaf()) return node->returnResults();

        if(newData[node->colToConsider] <= node->threshold) return predictionBackend(node->left, newData);
        else return predictionBackend(node->right, newData);
    }

    public: 

    DecisionTree(vector<vector<double>> data, double k, int yIndex) {

        cout<<"Decision Tree instance created!"<<endl;

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

        root = new Node(dataset, result, k, -1);
        treeSplit(root);
    }

    double predict(vector<double> newData) {
        return predictionBackend(root, newData);
    }
};
