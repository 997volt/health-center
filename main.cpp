#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;


const int WLDATA_COLS_NUMBER = 11;
//day of week when all averages for week are calculated
const int DAY_OF_WEEK_FOR_AVERAGES = 5; 
const char CSV_SEPARATOR = ';';
const int MAX_CALORIES_DIFF_IN_RUN = 600;

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
        size_t pos = data_row.find(CSV_SEPARATOR, 0);
        while(pos != string::npos)
        {
            positions.push_back(pos);
            pos = data_row.find(CSV_SEPARATOR,pos+1);
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
    WeeklyData(vector<WaistlineData> &wldata, tm date_done){
        int weight_datapoints = 0;
        int cal_datapoints = 0;
        weight = 0;
        calories = 0;
        for (int i = 0; i < wldata.size(); i++) {
            tm dataset_date = wldata[i].get_tm_date();
            if (is_date_after(dataset_date, date_done)){
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
                if (dataset_date.tm_wday == DAY_OF_WEEK_FOR_AVERAGES
                    || i == wldata.size()-1) {
                    weight = weight / weight_datapoints;
                    calories = calories / cal_datapoints;
                    date = dataset_date;
                    return;
                }
            }
        }
    }

    tm get_date(){
        return date;
    }

    float get_calories(){
        return calories;
    }

    float get_weight(){
        return weight;
    }

    void printdata(){
        cout << "date: " << date.tm_mday << "-" << date.tm_mon+1 << "-" << date.tm_year+1900 
        << ",  calories: " << calories 
        << ",  weight: " << weight 
        << endl;
    }
};

class WeightAnalisysData {
private:
    float avg_weight;
    float weight_diff_kg;
    float weight_diff_precentage;

    public:
    WeightAnalisysData(float _avg_weight, float _weight_diff_kg, float _weight_diff_precentage){
        avg_weight = _avg_weight;
        weight_diff_kg = _weight_diff_kg;
        weight_diff_precentage = _weight_diff_precentage;
    }

    WeightAnalisysData(float _avg_weight){
        avg_weight = _avg_weight;
        weight_diff_kg = 0;
        weight_diff_precentage = 0;
    }

    float get_avg_weight(){
        return avg_weight;
    }
};

bool is_date_after(tm tocheck, tm relative_date){
    return ((tocheck.tm_year > relative_date.tm_year)
                ||(tocheck.tm_year == relative_date.tm_year && tocheck.tm_yday > relative_date.tm_yday));
}

bool is_date_same(tm tocheck, tm relative_date){
    return (tocheck.tm_year == relative_date.tm_year && tocheck.tm_yday == relative_date.tm_yday);
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

void calculate_weekly_data(vector<WaistlineData> &wldata, vector<WeeklyData> &weekly_data){
    tm date_done = {};
    bool exit = false;
    do {
        WeeklyData calc_data = WeeklyData(wldata, date_done);
        date_done = calc_data.get_date();
        weekly_data.push_back(calc_data);
        exit = is_date_same(wldata[wldata.size()-1].get_tm_date(), date_done);
    }while (!exit);
}

void get_avg_weights(vector<WaistlineData> &wldata, vector<WeightAnalisysData> &avg_weight_vector, int num_of_weeks){
    float combined_weight = 0;
    int weight_datapoints = 0;
    for (int day = 0; day < num_of_weeks*7; day++){
        int data_index = wldata.size() - day - 1;
        float wlweight = wldata[data_index].get_weight();
        if (wlweight != 0){
            combined_weight += wlweight;
            weight_datapoints += 1;
        }
        if (day % 7 == 6) {
            float avg_weight = combined_weight/weight_datapoints;
            avg_weight_vector.push_back(WeightAnalisysData(avg_weight));
            combined_weight = 0;
            weight_datapoints = 0;
        }
    }
}

void analyse_last_weeks(vector<WaistlineData> &wldata){
    vector<WeightAnalisysData> avg_weight;
    get_avg_weights(wldata, avg_weight, 3);
    for (int i = 0; i < avg_weight.size(); i++){
        cout << "avg_weight[" << i << "] = " << avg_weight[i].get_avg_weight() << endl;
    }
}

int last_run_count(vector<WeeklyData> &weekly_data){
    int num_of_items = weekly_data.size();
    int num_of_counted = 1;
    float avg_calories = weekly_data[num_of_items-1].get_calories(); 
    for (int i = num_of_items-2; i > 0; i--) {
        float this_item = weekly_data[i].get_calories();
        if (abs(this_item - avg_calories) > MAX_CALORIES_DIFF_IN_RUN){
            break;
        }
        avg_calories = (avg_calories*num_of_counted + this_item)/(num_of_counted+1); 
        num_of_counted++;
    }
    return num_of_counted;
}


int main() {
    vector<WaistlineData> wldata;
    vector<WeeklyData> weekly_data;
    read_wl_data("in/diary_export.csv", wldata);
    analyse_last_weeks(wldata);
    /*
    calculate_weekly_data(wldata, weekly_data);
    int last_run_length = last_run_count(weekly_data);
    int last_run_start_index = weekly_data.size()-last_run_length;
    float last_run_weight_diff = weekly_data[weekly_data.size()-1].get_weight() 
        - weekly_data[last_run_start_index-1].get_weight();
    float last_run_weight_diff_per_week = last_run_weight_diff/last_run_length;
    
    for (int i = last_run_start_index; i < weekly_data.size(); i++) {
        weekly_data[i].printdata();
    }  
    cout << "Last run length (weeks): " << last_run_length << endl;
    cout << "Weight diff: " << last_run_weight_diff << endl;
    cout << "Weight diff per week: " << last_run_weight_diff_per_week << endl;
    */

    return 0;
}