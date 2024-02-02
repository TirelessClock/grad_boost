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

class TrainNode {

    vector<vector<double>> dataset; 
    vector<double> result;
    
    double allowedDepth;
    double numRows;
    double numColumns;
    double giniImpurity;

    public:

    TrainNode* left; 
    TrainNode* right; 
    int colToConsider; 
    double threshold;

    TrainNode(vector<vector<double>> dataset, vector<double> result, double k, double giniImpurity) {
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
                if(dataLeft.size() > 0) left = new TrainNode(dataLeft, resultLeft, allowedDepth-1, gILeft);
                if(dataRight.size() > 0) right = new TrainNode(dataRight, resultRight, allowedDepth-1, gIRight);
                
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

class Node {
    
    public: 

    Node* left; 
    Node* right;
    
    int colToConsider;
    double threshold;
    double res;

    Node(int colToConsider, double threshold) {
        this->colToConsider = colToConsider;
        this->threshold = threshold;

        left = nullptr;
        right = nullptr;
        res = -1;
    }

    bool isLeaf() {
        return (left == nullptr and right == nullptr);
    }

};

class DecisionTree {

    TrainNode* TrainRoot;
    Node* root; 

    Node* createTree(TrainNode* TrainNode) {
        Node* node = new Node(TrainNode->colToConsider, TrainNode->threshold);
        if(TrainNode->isLeaf()) {
            node->res = TrainNode->returnResults();
        }
        if(TrainNode->left) node->left = createTree(TrainNode->left);
        if(TrainNode->right) node->right = createTree(TrainNode->right);
        
        delete TrainNode;
        return node;
    }
    
    void treeSplit(TrainNode* TrainRoot) {
        
        if(!TrainRoot) return;
        
        TrainRoot->determineSplit();
        
        treeSplit(TrainRoot->left);
        treeSplit(TrainRoot->right);
    }

    double predictionBackend(Node* Node, vector<double>& newData) {
        if(Node->isLeaf()) return Node->res;

        if(newData[Node->colToConsider] <= Node->threshold) return predictionBackend(Node->left, newData);
        else return predictionBackend(Node->right, newData);
    }

    void printBackend(Node* node) {
        if(!node) return;
        if(node->isLeaf()) {
            cout<<node<<": "<<node->colToConsider<<", "<<node->threshold<<" Leaf Reached."<<endl;
            return;
        }
        cout<<node<<": "<<node->colToConsider<<", "<<node->threshold<<endl;

        cout<<"Left: ";
        printBackend(node->left);
        cout<<"Right: ";
        printBackend(node->right);
    }

    public: 

    DecisionTree(vector<vector<double>> dataset, vector<double> result, double k) {

        cout<<"Decision Tree instance created!"<<endl;
        
        TrainRoot = new TrainNode(dataset, result, k, -1);
        treeSplit(TrainRoot);
        root = createTree(TrainRoot);
    }

    DecisionTree(vector<vector<double>> data, int yIndex, double k) {
        
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

        TrainRoot = new TrainNode(dataset, result, k, -1);
        treeSplit(TrainRoot);
        root = createTree(TrainRoot);
    }

    double predict(vector<double> newData) {
        return predictionBackend(root, newData);
    }

    void printTree() {
        printBackend(root);
    }
};
