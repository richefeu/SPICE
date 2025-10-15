#pragma once

#include <algorithm> // For std::sort and std::lower_bound
#include <array>
#include <iostream>
#include <vector>

#include "toofus-gate/nlohmann/json.hpp"

template <typename T> class propertyProfile {
public:
  // Vector of (x, y) pairs
  std::vector<std::array<T, 2>> values;
  
  void setConstant(T value) {
    values.clear();
    values.push_back({(T)0, value});
  }

  // Reads a JSON value and populates the 'values' vector.
  // The JSON can be either:
  // 1. A single value (e.g., 1.0), which is interpreted as [[0.0, 1.0]].
  // 2. An array of (x, y) pairs (e.g., [[0.4, 1.0], [0.6, 2.0]]).
  void readJson(const nlohmann::json &j) {
    values.clear(); // Clear existing values

   if (j.is_number()) {
      // Case: Single value (e.g., "cohesion": 1.0)
      values.push_back({(T)0.0, j.get<T>()});
    } else if (j.is_array()) {
      // Case: Array of (x, y) pairs (e.g., "cohesion": [[0.4, 1.0], [0.6, 2.0]])
      for (const auto &entry : j) {
        if (entry.is_array() && entry.size() == 2) {
          values.push_back({entry[0].get<T>(), entry[1].get<T>()});
        } else {
          std::cerr << "Invalid format for (x, y) pair. Skipping entry." << std::endl;
        }
      }
      sort();
    } else {
      std::cerr << "Unsupported JSON format for propertyProfile." << std::endl;
    }
    
  }

  // Sorts the 'values' in ascending order of x
  void sort() {
    std::sort(values.begin(), values.end(),
              [](const std::array<T, 2> &a, const std::array<T, 2> &b) { return a[0] < b[0]; });
  }

  T getMaxValue() const {
    if (values.empty()) {
      std::cout << "No data points in property profile" << std::endl;
      return (T)0;
    }

    T maxValue = values[0][1];
    for (const auto &pair : values) {
      if (pair[1] > maxValue) { maxValue = pair[1]; }
    }
    return maxValue;
  }
  
  T getMinValue() const {
    if (values.empty()) {
      std::cout << "No data points in property profile" << std::endl;
      return (T)0;
    }

    T minValue = values[0][1];
    for (const auto &pair : values) {
      if (pair[1] < minValue) { minValue = pair[1]; }
    }
    return minValue;
  }

  // Interpolates a value at a given x
  T getValueAt(T x) const {
    if (values.empty()) {
      std::cout << "No data points in property profile" << std::endl;
      return (T)0;
    }

    // Find the appropriate range for interpolation
    auto comparator = [](const std::array<T, 2> &a, T v) { return a[0] < v; };
    auto it         = std::lower_bound(values.begin(), values.end(), x, comparator);

    // Handle edge cases
    if (it == values.begin()) {
      // x is less than or equal to the first x value
      return it->at(1);
    } else if (it == values.end()) {
      // x is greater than or equal to the last x value
      return values.back().at(1);
    }

    // Perform linear interpolation
    const auto &prev = *(it - 1);
    const auto &next = *it;

    return prev[1] + (next[1] - prev[1]) * ((x - prev[0]) / (next[0] - prev[0]));
  }

  // Overload the << operator to print the propertyProfile
  friend std::ostream &operator<<(std::ostream &os, const propertyProfile<T> &profile) {
    os << "propertyProfile values:\n";
    for (const auto &entry : profile.values) { os << "  (x: " << entry[0] << ", y: " << entry[1] << ")\n"; }
    return os;
  }
};
