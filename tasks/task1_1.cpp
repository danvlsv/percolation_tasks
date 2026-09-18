#include <iostream>
#include <string>
#include <set>
#include <fmt/format.h>
#include <print>
#include <ranges>
#include <format>

#include "percolation/data/percolation_system.hpp"
#include "percolation/utils/parser.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  try
  {
    const auto input = percolation::utils::Parser::GetPercolationInput(argv[1]);
    if (!input)
    {
      return 1;
    }

    std::cout << fmt::format(
      "Running {} trials, matrix size: {}, occupancy probability: {}\n",
      input->trials,
      input->linearSize,
      input->occupancyProbability);

    for (std::size_t trial = 0; trial < input->trials; ++trial) {

      percolation::data::PercolationSystem system(
        input->linearSize,
        input->occupancyProbability,
        static_cast<std::uint32_t>(trial + 1));

      const auto & matrix = system.Matrix();

      std::cout << fmt::format("Trial {}\n", trial + 1);

      for (std::size_t row = 0; row < matrix.Rows; ++row) {
        for (std::size_t column = 0; column < matrix.Columns; ++column)
        {
          std::cout << matrix(row, column) << (column + 1 == matrix.Columns ? '\n' : ' ');
        }
      }

      std::cout << '\n';

    }

  } catch (const std::exception & exception) {
    std::cerr << "Error: " << exception.what() << '\n';
    return 1;
  }

  return 0;
}