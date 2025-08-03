#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_ROWS = 1000;


class WaistlineData {
private:

    const int COLS_NUMBER = 11;
    string date;
    string calories;
    string protein;
    string weight;
    string bodyfat;
    string musclemass;

    vector<size_t> get_comma_positions(string data_row){
        vector<size_t> positions;
        size_t pos = data_row.find(',', 0);
        while(pos != string::npos)
        {
            positions.push_back(pos);
            pos = data_row.find(',',pos+1);
        }
        return positions;
    }

public:
    WaistlineData() {
    }

    void set_all(string data_row){
        vector<size_t> pos = get_comma_positions(data_row);
        date = data_row.substr(0,pos[0]);
        calories = data_row.substr(pos[0]+1,pos[1]-pos[0]-1);
    }

    void printdata(){
        cout << "date: " << date << ", weight: " << weight << endl;
    }
};

int main() {
    ifstream file("in/diary_export.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    WaistlineData wldata[MAX_ROWS];
    string line;
    int row = 0;
    // Store the CSV data from the CSV file to the 2D array
    while (getline(file, line) && row < MAX_ROWS) {
        if(row != 0){
            wldata[row].set_all(line); 
        }
        row++;
    }
    // close the file after read opeartion is complete
    file.close();

    return 0;
}