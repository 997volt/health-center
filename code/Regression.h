#ifndef REGRESSION_H
#define REGRESSION_H

#include <vector>
#include "WaistlineData.h"

//around 0.25% weekly
const float COEFFICENT_BULKING_MAX = 0.05f;
const float COEFFICENT_BULKING_MIN = 0.025f;

class Regression {
    private:
        // function formula: y = ax + b
        std::vector<float> x;
        std::vector<float> y;
        float a;
        float b;
        float sum_x;
        float sum_y;
        float sum_xx;
        float sum_xy;
        float sum_yy;
        void calculate_sums();
        void calculate_a(); // a is a coefficent of calculated function
        void calculate_b(); // b is contant term of calculated function
    
    public:
        Regression(std::vector<WaistlineData> &wldata, int num_of_measurements);
        float get_coefficent() const;
        float get_contant_term() const;
        void print_function() const;
};

void analyse_regression(float coefficent);


#endif // REGRESSION_H