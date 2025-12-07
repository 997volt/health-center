#include <cmath>
#include <iostream>
#include <ostream>
#include <vector>

#include "Regression.h"


void Regression::calculate_sums(){
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
void Regression::calculate_a(){
    long num_of_datapoints = x.size();
    float numerator = (float)num_of_datapoints * sum_xy - sum_x * sum_y;
    float denominator = (float)num_of_datapoints * sum_xx - sum_x * sum_x;
    a = numerator / denominator;
}

// b is contant term of calculated function
void Regression::calculate_b(){
    long num_of_datapoints = x.size();
    float numerator = sum_y * sum_xx - sum_x * sum_xy;
    float denominator = (float)num_of_datapoints * sum_xx - sum_x * sum_x;
    b = numerator / denominator;
}

void Regression::calculate_regression(){
    calculate_sums();
    calculate_a();
    calculate_b();
}

void Regression::remove_outliers(){
    float highest_diff = 0;
    int highest_diff_index = 0;
    print_data();
    calculate_regression();

    for (int day = 0; day < x.size(); day++){
        float calc_y = a * x[day] + b;
        float diff = std::fabs(y[day] - calc_y);
        if(diff > highest_diff){
            highest_diff = diff;
            highest_diff_index = day;
        }
    }
    if(highest_diff > 1){
        y.erase(y.begin() + highest_diff_index);
        x.erase(x.begin() + highest_diff_index);
    }
    print_data();
}

    
Regression::Regression(std::vector<WaistlineData> &wldata, int num_of_measurements){
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
    remove_outliers();
    calculate_regression();
}

float Regression::get_coefficent() const{
    return a;
}

float Regression::get_contant_term() const{
    return b;
}

void Regression::print_function() const{
    std::cout << "y = " << a << "x + " << b << std::endl;
}

void Regression::print_data() const{
    for (int day = 0; day < x.size(); day++){
        std::cout << day << ": x = " << x[day] << " y = " << y[day] << std::endl;
    }
}

void analyse_regression(float coefficent){
    if(coefficent > COEFFICENT_BULKING_MAX){
        std::cout << "Coefficent > " << COEFFICENT_BULKING_MAX << "; too many calories for bulking" << std::endl;
    }
    else if (coefficent > COEFFICENT_BULKING_MIN){
        std::cout << COEFFICENT_BULKING_MIN << " < Coefficent < " << COEFFICENT_BULKING_MAX << "; ideal range for bulking" << std::endl;
    }
    else{
        std::cout << "Coefficent < " << COEFFICENT_BULKING_MIN << "; too little calories for bulking" << std::endl;
    }
}