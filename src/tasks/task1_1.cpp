#include <string>
#include <iostream>

#include <fmt/format.h>

#include "percolation/utils/parser.hpp"
#include "percolation/data/matrix.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <matrix_file_path>\n";
    return 1;
  }

  std::string filePath = argv[1];
  auto matrixOpt = percolation::utils::Parser::GetDefinedConcetrationData<uint8_t>(filePath);

  if (!matrixOpt.has_value())
  {
    std::cerr << "Failed to parse matrix from file: " << filePath << "\n";
    return 1;
  }

  const auto & matrix = matrixOpt.value();
  std::cout << fmt::format(
    "Matrix loaded successfully with dimensions: {}x{}\n",
    matrix.Rows,
    matrix.Columns
  );

  return 0;
}