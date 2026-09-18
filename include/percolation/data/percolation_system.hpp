#ifndef PERCOLATION__DATA__PERCOLATION_SYSTEM_HPP_
#define PERCOLATION__DATA__PERCOLATION_SYSTEM_HPP_

#include <cstddef>
#include <random>
#include <stdexcept>
#include <utility>

#include "percolation/data/matrix.hpp"

namespace percolation::data
{

struct PercolationInput
{
  std::size_t linearSize;
  double occupancyProbability;
  std::size_t trials = 1;
};

class PercolationSystem
{
public:
  PercolationSystem(size_t linearSize, double occupancyProbability)
  : PercolationSystem(linearSize, occupancyProbability, std::random_device{}())
  {
  }

  PercolationSystem(size_t linearSize, double occupancyProbability, std::uint32_t seed)
  : m_size(linearSize),
    m_probability(occupancyProbability),
    m_generator(seed),
    m_matrix(linearSize, linearSize)
  {
    if (m_size == 0) {
      throw std::invalid_argument("Lattice size must be greater than zero.");
    }

    if (m_probability < 0.0 || m_probability > 1.0) {
      throw std::invalid_argument("Occupation probability p must be in [0, 1].");
    }

    FillRandom(occupancyProbability);
  }

  void FillRandom(double occupancyProbability)
  {
    if (occupancyProbability < 0.0 || occupancyProbability > 1.0) {
      throw std::invalid_argument("Occupation probability p must be in [0, 1].");
    }

    const size_t totalSites = m_size * m_size;
    const size_t fieldsToFill = static_cast<size_t>(
      std::llround(occupancyProbability * static_cast<double>(totalSites)));

    Reset();

    std::vector<size_t> chosen;
    chosen.reserve(fieldsToFill);
    for (size_t i = 0; i < totalSites; ++i) {
      if (chosen.size() < fieldsToFill) {
        chosen.push_back(i);
      } else {
        std::uniform_int_distribution<size_t> dist(0, i);
        const size_t j = dist(m_generator);
        if (j < fieldsToFill) {
          chosen[j] = i;
        }
      }
    }

    m_probability = occupancyProbability;
    for (const auto index : chosen) {
      m_matrix(index / m_size, index % m_size) = 1;
    }
  }

  inline void Reset() noexcept
  {
    for (size_t row = 0; row < m_size; ++row) {
      for (size_t column = 0; column < m_size; ++column) {
        m_matrix(row, column) = 0;
      }
    }
  }

  const Matrix2D<int> & Matrix() const
  {
    return m_matrix;
  }

  double OccupiedFraction() const
  {
    const size_t occupied = CountOccupiedNodes();
    return static_cast<double>(occupied) / static_cast<double>(m_size * m_size);
  }

  size_t CountOccupiedNodes() const
  {
    size_t occupied = 0;
    for (size_t row = 0; row < m_size; ++row)
    {
      for (size_t column = 0; column < m_size; ++column) {
        occupied += m_matrix(row, column) == 1 ? 1u : 0u;
      }
    }
    return occupied;
  }

  inline size_t GetSize() const
  {
    return m_size;
  }

  inline double GetProbability() const
  {
    return m_probability;
  }

private:
  size_t m_size;
  double m_probability;
  std::mt19937 m_generator;
  Matrix2D<int> m_matrix;
};

} // namespace percolation::data

#endif // PERCOLATION__DATA__PERCOLATION_SYSTEM_HPP_