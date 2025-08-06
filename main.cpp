#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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
    WaistlineData(string data_row) {
        vector<size_t> comma_positions = get_comma_positions(data_row);

        date = read_element(data_row, comma_positions, 0);
        calories = read_element(data_row, comma_positions, 1);
        protein = read_element(data_row, comma_positions, 6);
        weight = read_element(data_row, comma_positions, 8);
        bodyfat = read_element(data_row, comma_positions, 9);
        musclemass = read_element(data_row, comma_positions, 10);
    }

    void printdata(){
        cout << "date: " << date 
        << ",  calories: " << calories 
        << ",  protein: " << protein 
        << ",  weight: " << weight 
        << ",  bodyfat: " << bodyfat 
        << ",  musclemass: " << musclemass
        << endl;
    }
};

ifstream open_wl_file(string path){
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Error opening file");
    }
    return file;
}


int main() {
    vector<WaistlineData> wldata;
    ifstream file = open_wl_file("in/diary_export.csv");

    string line;
    int row = 0;
    while (getline(file, line)) {
        if(row != 0){
            wldata.push_back(WaistlineData(line));
        }
        row++;
    }
    // close the file after read opeartion is complete
    file.close();

    for (int i = 0; i < wldata.size(); i++) {
        wldata[i].printdata();
    }  

    return 0;
}