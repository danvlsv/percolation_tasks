#include <iostream>
#include <string>

#include <fmt/format.h>

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

    double totalConcentration = 0.0;
    for (std::size_t trial = 0; trial < input->trials; ++trial)
    {
      percolation::data::PercolationSystem system(
          input->linearSize,
          input->occupancyProbability,
          static_cast<std::uint32_t>(trial + 1));
      const auto& matrix = system.Matrix();

      std::cout << fmt::format("Trial {}\n", trial + 1);
      for (std::size_t row = 0; row < matrix.Rows; ++row)
      {
        for (std::size_t column = 0; column < matrix.Columns; ++column)
        {
          std::cout << matrix(row, column)
                    << (column + 1 == matrix.Columns ? '\n' : ' ');
        }
      }

      std::cout << fmt::format(
        "Occupied fraction p = {}\n\n",
        system.OccupiedFraction());

      totalConcentration += system.OccupiedFraction();
    }

    const double averageConcentration =
        totalConcentration / static_cast<double>(input->trials);
    std::cout << fmt::format(
        "Average concentration p_avg = {}\n",
        averageConcentration);
  }
  catch (const std::exception & exception)
  {
    std::cerr << "Error: " << exception.what() << '\n';
    return 1;
  }

  return 0;
}