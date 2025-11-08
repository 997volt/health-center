#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

#include "WaistlineData.h"

using namespace std;

const int DAYS_TO_ANALYSE = 10;

//around 0.25% weekly
const float COEFFICENT_BULKING_MAX = 0.05f;
const float COEFFICENT_BULKING_MIN = 0.025f;

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
            long num_of_datapoints = x.size();
            float numerator = (float)num_of_datapoints * sum_xy - sum_x * sum_y;
            float denominator = (float)num_of_datapoints * sum_xx - sum_x * sum_x;
            a = numerator / denominator;
        }

        // b is contant term of calculated function
        void calculate_b(){
            long num_of_datapoints = x.size();
            float numerator = sum_y * sum_xx - sum_x * sum_xy;
            float denominator = (float)num_of_datapoints * sum_xx - sum_x * sum_x;
            b = numerator / denominator;
        }

    
    public:
        Regression(vector<WaistlineData> &wldata, int num_of_measurements){
            if(num_of_measurements <= wldata.size()){
                for (int day = 0; day < num_of_measurements; day++){
                    long data_index = wldata.size() - num_of_measurements + day;
                    float wlweight = wldata[data_index].get_weight();
                    if (wlweight != 0){
                        x.push_back((float)day);
                        y.push_back(wlweight);
                    }
                }
            }
            calculate_sums();
            calculate_a();
            calculate_b();
        }

        float get_coefficent() const{
            return a;
        }

        float get_contant_term() const{
            return b;
        }

        void print_function() const{
            cout << "y = " << a << "x + " << b << endl;
        }
};

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

int main() {
    vector<WaistlineData> wldata;
    read_wl_data("in/diary_export.csv", wldata);
    auto regression = Regression(wldata, DAYS_TO_ANALYSE);
    regression.print_function();
    analyse_regression(regression.get_coefficent());

    int avg_calories = get_average_calories(wldata, DAYS_TO_ANALYSE);
    float avg_bodyfat = get_average_bodyfat(wldata, DAYS_TO_ANALYSE);
    cout << "avg_calories = " << avg_calories << endl;
    cout << "avg_bodyfat = " << avg_bodyfat << "%" << endl;
    if(avg_bodyfat > 20){
        cout << "Your bodyfat is greater that 20%, you should be cutting";
    }
    return 0;
}