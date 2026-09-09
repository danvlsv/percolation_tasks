#ifndef PERCOLATION__DATA__MATRIX_HPP_
#define PERCOLATION__DATA__MATRIX_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

namespace percolation::data
{

template<typename T>
struct Matrix2D
{
  Matrix2D() : Rows(0), Columns(0), m_data() {}

  Matrix2D(size_t r, size_t c) : Rows(r), Columns(c), m_data(r * c, T{}) {}

  T& operator()(size_t r, size_t c)
  {
    return m_data[r * Columns + c];
  }

  const T& operator()(size_t r, size_t c) const
  {
    return m_data[r * Columns + c];
  }

  T& at(size_t r, size_t c)
  {
    return m_data.at(r * Columns + c);
  }

  const T& at(size_t r, size_t c) const
  {
    return m_data.at(r * Columns + c);
  }

  void Fill(const T& value)
  {
    std::fill(m_data.begin(), m_data.end(), value);
  }

  size_t Rows;
  size_t Columns;

private:
  std::vector<T> m_data;
};

} // namespace percolation::data

#endif // PERCOLATION__DATA__MATRIX_HPP_