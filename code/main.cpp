#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

#include "WaistlineData.h"
#include "Regression.h"

using namespace std;

const int DAYS_TO_ANALYSE = 10;

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