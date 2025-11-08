#ifndef WAISTLINE_DATA_H
#define WAISTLINE_DATA_H

#include <ctime>
#include <string>
#include <vector>

const char CSV_SEPARATOR = ';';
const int WLDATA_COLS_NUMBER = 11;

class WaistlineData {
private:
    tm date;
    int calories;
    float protein;
    float weight;
    float bodyfat;
    float musclemass;
    std::vector<size_t>
    get_comma_positions(std::string_view const &data_row) const;
    std::string read_element(std::string_view const &data_row,
                             std::vector<size_t> comma_positions,
                             size_t element_number) const;
    tm get_date(std::string const &date_string) const;

public:
    explicit WaistlineData(std::string_view const &data_row);
    void printdata() const;
    tm get_tm_date() const;
    int get_calories() const;
    float get_bodyfat() const;
    float get_weight() const;
};


#endif // WAISTLINE_DATA_H