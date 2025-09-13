#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int WLDATA_COLS_NUMBER = 11;
const char CSV_SEPARATOR = ';';
const int DAYS_TO_ANALYSE = 10;

//around 0.25% weekly
const float COEFFICENT_BULKING_MAX = 0.05;
const float COEFFICENT_BULKING_MIN = 0.025;

ifstream open_wl_file(string path);

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

class Regression {
    private:
        vector<float> x;
        vector<float> y;

        // from formula y = ax + b
        float a;
        float b;
        float sum_x;
        float sum_y;
        float sum_xx;
        float sum_xy;
        float sum_yy;

        void calculate_sums(){
            sum_x = 0;
            sum_y = 0;
            sum_xx = 0;
            sum_xy = 0;
            sum_yy = 0;
            for (int i = 0; i < x.size(); i++){
                float this_x = x[i];
                float this_y = y[i];
                sum_x += this_x;
                sum_y += this_y;
                sum_xx += this_x*this_x;
                sum_xy += this_x*this_y;
                sum_yy += this_y*this_y;
            }
        }

        // a is a coefficent of calculated function
        void calculate_a(){
            float num_of_datapoints = x.size();
            float numerator = num_of_datapoints * sum_xy - sum_x * sum_y;
            float denominator = num_of_datapoints * sum_xx - sum_x * sum_x;
            a = numerator / denominator;
        }

        // b is contant term of calculated function
        void calculate_b(){
            float num_of_datapoints = x.size();
            float numerator = sum_y * sum_xx - sum_x * sum_xy;
            float denominator = num_of_datapoints * sum_xx - sum_x * sum_x;
            b = numerator / denominator;
        }

    
    public:
        Regression(vector<WaistlineData> &wldata, int num_of_measurements){
            if(num_of_measurements <= wldata.size()){
                for (int day = 0; day < num_of_measurements; day++){
                    int data_index = wldata.size() - num_of_measurements + day;
                    float wlweight = wldata[data_index].get_weight();
                    if (wlweight != 0){
                        x.push_back(day);
                        y.push_back(wlweight);
                    }
                }
            }
            calculate_sums();
            calculate_a();
            calculate_b();
        }

        float get_coefficent(){
            return a;
        }

        float get_contant_term(){
            return b;
        }

        void print_function(){
            cout << "y = " << a << "x + " << b << endl;
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

void analyse_regression(float coefficent){
    if(coefficent > COEFFICENT_BULKING_MAX){
        cout << "Coefficent > " << COEFFICENT_BULKING_MAX << "; too many calories for bulking" << endl;
    }
    else if (coefficent > COEFFICENT_BULKING_MIN){
        cout << COEFFICENT_BULKING_MIN << " < Coefficent < " << COEFFICENT_BULKING_MAX << "; ideal range for bulking" << endl;
    }
    else{
        cout << "Coefficent < " << COEFFICENT_BULKING_MIN << "; too little calories for bulking" << endl;
    }
}

float get_average_calories(vector<WaistlineData> &wldata, int num_of_measurements){
    float sum_calories = 0;
    float num_of_found_measurements = 0;
    float avg_calories;
    if(num_of_measurements <= wldata.size()){
        for (int day = 0; day < num_of_measurements; day++){
            int data_index = wldata.size() - num_of_measurements + day;
            float wlcaloreis = wldata[data_index].get_calories();
            if (wlcaloreis != 0){
                sum_calories += wlcaloreis;
                num_of_found_measurements++;
            }
        }
    }
    if(num_of_found_measurements > 0){
        avg_calories = sum_calories/num_of_found_measurements;
    }
    return avg_calories;
}

int main() {
    vector<WaistlineData> wldata;
    read_wl_data("in/diary_export.csv", wldata);
    Regression regression = Regression(wldata, DAYS_TO_ANALYSE);
    regression.print_function();
    analyse_regression(regression.get_coefficent());
    cout << "avg_calories = " << get_average_calories(wldata, DAYS_TO_ANALYSE) << endl;;
    return 0;
}