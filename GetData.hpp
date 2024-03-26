#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std; 

vector<vector<double>> preprocess(string location, bool hasIndex) {

    ifstream file("data.csv"); 

    if (!file.is_open()) {
        cout << "Error opening file\n";
        return {};
    }

    vector<vector<double>> data; 

    string line;
    int x = 0;
    double idk = 1;
    map<string, double> index; 

    while (getline(file, line)) {
        
        if(x == 0 && hasIndex) {
            x++;
            continue;
        }

        stringstream ss(line);
        vector<double> row;
        string value;
        bool flag = 1;

        while (getline(ss, value, ',')) { 
            try {
                double val = stod(value);
                row.push_back(val);
            }
            catch(const std::invalid_argument) {
                if(value.size() == 0) {
                    flag = 0;
                    break;
                }

                else {
                    if(index.find(value) == index.end()) {
                        index[value] = idk;
                        idk++;
                    }
                    row.push_back(index[value]);
                }
            }
        }

        if(flag) {
            data.push_back(row); 
        }

    }
    
    file.close();
    return data;

}


