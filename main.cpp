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

    string read_element(string data_row, vector<size_t> comma_positions, size_t element_number){
        if(element_number == 0){
            return data_row.substr(0,comma_positions[0]);;
        }
        else if (element_number == COLS_NUMBER-1) {
            return data_row.substr(
                comma_positions[element_number-1]+1, data_row.length());
        }
        else {
            return data_row.substr(
                comma_positions[element_number-1]+1,
                comma_positions[element_number]-comma_positions[element_number-1]-1);
        }
    }

public:
    WaistlineData() {
    }

    void set_all(string data_row){
        vector<size_t> comma_positions = get_comma_positions(data_row);

        date = read_element(data_row, comma_positions, 0);
        calories = read_element(data_row, comma_positions, 1);
        protein = read_element(data_row, comma_positions, 6);
        weight = read_element(data_row, comma_positions, 8);
        bodyfat = read_element(data_row, comma_positions, 9);
        musclemass = read_element(data_row, comma_positions, 10);
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