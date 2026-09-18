#ifndef PERCOLATION__UTILS__PARSER_HPP_
#define PERCOLATION__UTILS__PARSER_HPP_

#include <optional>
#include <string>
#include <string_view>

#include "percolation/data/matrix.hpp"
#include "percolation/data/percolation_system.hpp"

namespace percolation::utils
{

class Parser
{
public:
  Parser() = delete;

public:
  static std::optional<data::PercolationInput> GetPercolationInput(std::string_view filePath);

  template<typename T>
  static std::optional<data::Matrix2D<T>> GetDefinedConcetrationData(std::string_view filePath);

};

} // namespace percolation::utils

#endif // PERCOLATION__UTILS__PARSER_HPP_