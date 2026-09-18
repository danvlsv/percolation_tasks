#include "percolation/utils/parser.hpp"

#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <cstddef>

namespace percolation::utils
{

std::optional<data::PercolationInput> Parser::GetPercolationInput(std::string_view filePath)
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
std::optional<data::Matrix2D<T>> Parser::GetDefinedConcetrationData(std::string_view filePath)
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

template std::optional<data::Matrix2D<int>>
Parser::GetDefinedConcetrationData<int>(std::string_view);

template std::optional<data::Matrix2D<double>>
Parser::GetDefinedConcetrationData<double>(std::string_view);

} // namespace percolation::utils