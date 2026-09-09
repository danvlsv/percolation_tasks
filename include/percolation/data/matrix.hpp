#ifndef PERCOLATION__DATA__MATRIX_HPP
#define PERCOLATION__DATA__MATRIX_HPP

#include <vector>

namespace percolation::data
{

template<typename T>
struct Matrix2D
{

public:
  Matrix2D(size_t r, size_t c) : Rows(r), Columns(c), m_data(r * c) {}

  T& operator()(size_t r, size_t c) {
      return m_data[r * Rows + c];
  }
  const T& operator()(size_t r, size_t c) const {
      return m_data[r * Columns + c];
  }

public :
  size_t Rows;
  size_t Columns;

private:
  std::vector<T> m_data;

};



} // namespace percolation::data


#endif // PERCOLATION__DATA__MATRIX_HPP