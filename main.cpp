#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


const int WLDATA_COLS_NUMBER = 11;
//day of week when all averages for week are calculated
const int DAY_OF_WEEK_FOR_AVERAGES = 5; 

bool is_date_after(tm tocheck, tm relative_date);

class WaistlineData {
private:
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
        else if (element_number == WLDATA_COLS_NUMBER-1) {
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
        tm date = {};
        istringstream ss(date_string);
        ss >> get_time(&date, "%m/%d/%Y");
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
        cout << "date: " << date.tm_mday << "-" << date.tm_mon+1 << "-" << date.tm_year+1900 
        << ",  calories: " << calories 
        << ",  protein: " << protein 
        << ",  weight: " << weight 
        << ",  bodyfat: " << bodyfat 
        << ",  musclemass: " << musclemass
        << endl;
    }

    tm get_tm_date(){
        return date;
    }

    int get_calories(){
        return calories;
    }

    float get_weight(){
        return weight;
    }
};

// container for averaged weekly data
class WeeklyData {
private:
    tm date; //last day of data
    float weight;
    float calories;

public:
    WeeklyData(vector<WaistlineData> wldata, tm date_done){
        int weight_datapoints = 0;
        int cal_datapoints = 0;
        weight = 0;
        calories = 0;
        for (int i = 0; i < wldata.size(); i++) {
            tm date = wldata[i].get_tm_date();
            if (is_date_after(date, date_done)){
                float dataset_weight = wldata[i].get_weight();
                if (dataset_weight != 0) {
                    weight = weight + dataset_weight;
                    weight_datapoints++;
                }
                float dataset_calories = float(wldata[i].get_calories());
                if (dataset_calories != 0) {
                    calories = calories + dataset_calories;
                    cal_datapoints++;
                }
                if (date.tm_wday == DAY_OF_WEEK_FOR_AVERAGES) {
                    weight = weight / weight_datapoints;
                    calories = calories / cal_datapoints;
                    return;
                }
            }
        }
    }

    void printdata(){
        cout << "date: " << date.tm_mday << "-" << date.tm_mon+1 << "-" << date.tm_year+1900 
        << ",  calories: " << calories 
        << ",  weight: " << weight 
        << endl;
    }
};

bool is_date_after(tm tocheck, tm relative_date){
    return ((tocheck.tm_year > relative_date.tm_year)
                ||(tocheck.tm_year == relative_date.tm_year && tocheck.tm_yday > relative_date.tm_yday));
}

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