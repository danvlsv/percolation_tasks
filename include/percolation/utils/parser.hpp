#ifndef PERCOLATION__UTILS__PARSER_HPP
#define PERCOLATION__UTILS__PARSER_HPP

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include "percolation/data/matrix.hpp"

namespace percolation::utils
{

class Parser
{
public:
  template<typename T>
  static std::optional<data::Matrix2D<T>> GetDefinedConcetrationData(std::string_view filePath)
  {
    std::ifstream file(std::string(filePath), std::ios::in);
    if (!file)
    {
      std::cerr << fmt::format("Invalid file ({})\n", filePath);
      return std::nullopt;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (!(file >> rows >> cols))
    {
      std::cerr << fmt::format("Invalid matrix header in ({})\n", filePath);
      return std::nullopt;
    }

    if (rows == 0 || cols == 0)
    {
      std::cerr << fmt::format("Matrix dimensions must be greater than zero in ({})\n", filePath);
      return std::nullopt;
    }

    data::Matrix2D<T> matrix(rows, cols);

    for (size_t row = 0; row < rows; ++row)
    {
      for (size_t col = 0; col < cols; ++col)
      {
        T value{};
        if (!(file >> value))
        {
          std::cerr << fmt::format(
              "Not enough matrix entries in ({}) for size {}x{}\n",
              filePath,
              rows,
              cols);
          return std::nullopt;
        }

        matrix(row, col) = value;
      }
    }

    return matrix;
  }

};

} // namespace percolation::utils

#endif // PERCOLATION__UTILS__PARSER_HPP