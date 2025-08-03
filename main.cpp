#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_ROWS = 1000;
const int MAX_COLS = 10;

class WaistlineData {
private:

    string date;
    string calories;
    string protein;
    string weight;
    string bodyfat;
    string musclemass;

public:
    WaistlineData(string datarow) {
        cout << datarow << endl;
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

    // Define a 2D array to store the CSV data
    string data[MAX_ROWS][MAX_COLS];
    string line;
    int row = 0;
    // Store the CSV data from the CSV file to the 2D array
    while (getline(file, line) && row < MAX_ROWS) {
        stringstream ss(line);
        WaistlineData wldata = WaistlineData(line);
        string cell;
        int col = 0;
        while (getline(ss, cell, ',') && col < MAX_COLS) {
            data[row][col] = cell;
            col++;
        }
        row++;
    }
    // close the file after read opeartion is complete
    file.close();

    // Print the data stored in the 2D array
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < MAX_COLS && !data[i][j].empty();
             j++) {
            cout << data[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}