#ifndef PERCOLATION__UTILS__PARSER_HPP_
#define PERCOLATION__UTILS__PARSER_HPP_

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <cstddef>

#include <fmt/format.h>

#include "percolation/data/matrix.hpp"
#include "percolation/data/percolation_system.hpp"

namespace percolation::utils
{

class Parser
{
public:
  static std::optional<data::PercolationInput> GetPercolationInput(std::string_view filePath)
  {
    std::ifstream file(std::string(filePath), std::ios::in);
    if (!file)
    {
      std::cerr << fmt::format("Invalid file ({})\n", filePath);
      return std::nullopt;
    }

    data::PercolationInput input{};
    if (!(file >> input.linearSize >> input.occupancyProbability))
    {
      std::cerr << fmt::format(
          "Expected L and p in ({})\n",
          filePath);
      return std::nullopt;
    }

    if (file >> input.trials)
    {
      if (input.trials == 0)
      {
        std::cerr << "Number of trials must be greater than zero.\n";
        return std::nullopt;
      }
    }

    if (input.linearSize == 0)
    {
      std::cerr << "Lattice size L must be greater than zero.\n";
      return std::nullopt;
    }

    if (input.occupancyProbability < 0.0 ||
        input.occupancyProbability > 1.0)
    {
      std::cerr << "Occupation probability p must be in the interval [0, 1].\n";
      return std::nullopt;
    }

    return input;
  }

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

#endif // PERCOLATION__UTILS__PARSER_HPP_