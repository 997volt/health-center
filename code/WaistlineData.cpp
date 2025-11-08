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
