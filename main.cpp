#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class WaistlineData {
private:
    const int COLS_NUMBER = 11;
    tm date;
    int calories;
    float protein;
    float weight;
    float bodyfat;
    float musclemass;

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
        string result = "";
        if(element_number == 0){
            result = data_row.substr(0,comma_positions[0]);;
        }
        else if (element_number == COLS_NUMBER-1) {
            result = data_row.substr(
                comma_positions[element_number-1]+1, data_row.length());
        }
        else {
            result = data_row.substr(
                comma_positions[element_number-1]+1,
                comma_positions[element_number]-comma_positions[element_number-1]-1);
        }
        if(result == ""){
            result = "0";
        }
        return result;
    }

    tm get_date(string date_string){
        tm date;
        date.tm_mday = stoi(date_string.substr(3,2));
        date.tm_mon = stoi(date_string.substr(0,2));
        date.tm_year = stoi(date_string.substr(6,4));;
        return date;
    }

public:
    WaistlineData(string data_row) {
        vector<size_t> comma_positions = get_comma_positions(data_row);

        date = get_date(read_element(data_row, comma_positions, 0));
        calories = stoi(read_element(data_row, comma_positions, 1));
        protein = stof(read_element(data_row, comma_positions, 6));
        weight = stof(read_element(data_row, comma_positions, 8));
        bodyfat = stof(read_element(data_row, comma_positions, 9));
        musclemass = stof(read_element(data_row, comma_positions, 10));
    }

    void printdata(){
        cout << "date: " << date.tm_mday << "-" << date.tm_mon << "-" << date.tm_year 
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

void read_wl_data(string path, vector<WaistlineData> &wldata){
    ifstream file = open_wl_file(path);
    string line;
    while (getline(file, line)) {
        if(line.find("Date") == std::string::npos){
            wldata.push_back(WaistlineData(line));
        }
    }
    file.close();
}

int main() {
    vector<WaistlineData> wldata;
    read_wl_data("in/diary_export.csv", wldata);

    for (int i = 0; i < wldata.size(); i++) {
        wldata[i].printdata();
    }  

    return 0;
}