#include <ctime>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

#include "WaistlineData.h"

std::vector<size_t>
WaistlineData::get_comma_positions(std::string_view const &data_row) const {
  std::vector<size_t> positions;
  size_t pos = data_row.find(CSV_SEPARATOR, 0);
  while (pos != std::string::npos) {
    positions.push_back(pos);
    pos = data_row.find(CSV_SEPARATOR, pos + 1);
  }
  return positions;
}

std::string WaistlineData::read_element(std::string_view const &data_row,
                                        std::vector<size_t> comma_positions,
                                        size_t element_number) const {
  std::string result = "";
  if (element_number == 0) {
    result = data_row.substr(0, comma_positions[0]);
  } else if (element_number == WLDATA_COLS_NUMBER - 1) {
    result = data_row.substr(comma_positions[element_number - 1] + 1,
                             data_row.length());
  } else {
    result = data_row.substr(comma_positions[element_number - 1] + 1,
                             comma_positions[element_number] -
                                 comma_positions[element_number - 1] - 1);
  }
  if (result == "") {
    result = "0";
  }
  return result;
}

tm WaistlineData::get_date(std::string const &date_string) const {
  tm ret_date = {};
  std::istringstream ss(date_string);
  ss >> std::get_time(&ret_date, "%m/%d/%Y");
  return ret_date;
}

WaistlineData::WaistlineData(std::string_view const &data_row) {
  std::vector<size_t> comma_positions = get_comma_positions(data_row);

  date = get_date(read_element(data_row, comma_positions, 0));
  calories = std::stoi(read_element(data_row, comma_positions, 1));
  protein = std::stof(read_element(data_row, comma_positions, 6));
  weight = std::stof(read_element(data_row, comma_positions, 8));
  bodyfat = std::stof(read_element(data_row, comma_positions, 9));
  musclemass = std::stof(read_element(data_row, comma_positions, 10));
}

void WaistlineData::printdata() const{
  std::cout << "date: " << date.tm_mday << "-" << date.tm_mon + 1 << "-"
            << date.tm_year + 1900 << ",  calories: " << calories
            << ",  protein: " << protein << ",  weight: " << weight
            << ",  bodyfat: " << bodyfat << ",  musclemass: " << musclemass
            << std::endl;
}

tm WaistlineData::get_tm_date() const{
    return date;
}

int WaistlineData::get_calories() const{
    return calories;
}

float WaistlineData::get_bodyfat() const{
    return bodyfat;
}

float WaistlineData::get_weight() const{
    return weight;
}

std::ifstream open_wl_file(std::string const& path){
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::invalid_argument("Error opening file");
    }
    return file;
}

void read_wl_data(std::string const& path, std::vector<WaistlineData> &wldata){
    std::ifstream file = open_wl_file(path);
    std::string line;
    while (getline(file, line)) {
        if(line.find("Date") == std::string::npos){
            wldata.emplace_back(line);
        }
    }
    file.close();
}

int get_average_calories(std::vector<WaistlineData> &wldata, int num_of_measurements){
    float sum_calories = 0;
    float num_of_found_measurements = 0;
    int avg_calories = 0;
    if(num_of_measurements <= wldata.size()){
        for (int day = 0; day < num_of_measurements; day++){
            long data_index = wldata.size() - num_of_measurements + day;
            int wlcaloreis = wldata[data_index].get_calories();
            if (wlcaloreis != 0){
                sum_calories += (float)wlcaloreis;
                num_of_found_measurements++;
            }
        }
    }
    if(num_of_found_measurements > 0){
        avg_calories = (int)(sum_calories/num_of_found_measurements);
    }
    return avg_calories;
}

float get_average_bodyfat(std::vector<WaistlineData> &wldata, int num_of_measurements){
    float sum_bodyfat = 0;
    float num_of_found_measurements = 0;
    float avg_bodyfat = 0;
    if(num_of_measurements <= wldata.size()){
        for (int day = 0; day < num_of_measurements; day++){
            int data_index = (int)wldata.size() - num_of_measurements + day;
            float wl_bodyfat = wldata[data_index].get_bodyfat();
            if (wl_bodyfat != 0){
                sum_bodyfat += wl_bodyfat;
                num_of_found_measurements++;
            }
        }
    }
    if(num_of_found_measurements > 0){
        avg_bodyfat = sum_bodyfat/num_of_found_measurements;
    }
    return avg_bodyfat;
}

