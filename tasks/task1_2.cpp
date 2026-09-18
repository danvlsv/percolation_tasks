
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <execution>
#include <numeric>
#include <vector>

#include <fmt/format.h>

#include "percolation/data/percolation_system.hpp"

constexpr std::size_t TRIALS = 100;

constexpr std::initializer_list<std::size_t> LS = {
  1000, 2500, 5000
};

constexpr std::initializer_list<double> PS = {
  0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9
};

std::size_t ScottBins(std::size_t n)
{
  if (n == 0) return 1;
  const auto k = static_cast<std::size_t>(
    std::floor(std::pow(2.0 * static_cast<double>(n) / 3.0, 1.0 / 6.0)));
  const std::size_t m = (k + 1) * (k + 1) * (k + 1);
  return std::max<std::size_t>(m, 1);
}

std::vector<std::size_t> CountBins(const percolation::data::Matrix2D<int> & matrix,
                                   std::size_t bins)
{
  const std::size_t rows  = matrix.Rows;
  const std::size_t cols  = matrix.Columns;
  const std::size_t total = rows * cols;

  std::vector<std::size_t> counts(bins, 0);

  for (std::size_t r = 0; r < rows; ++r) {
    const std::size_t rowBase = r * cols;
    for (std::size_t c = 0; c < cols; ++c) {
      if (matrix(r, c) == 1) {
        const std::size_t idx = rowBase + c;
        std::size_t bin = idx * bins / total;
        if (bin >= bins) bin = bins - 1;
        ++counts[bin];
      }
    }
  }

  return counts;
}


double ChiSquare(const std::vector<std::size_t>& counts)
{
  const std::size_t total =
    std::accumulate(counts.begin(), counts.end(), std::size_t{0});
  const std::size_t m = counts.size();
  if (m == 0 || total == 0) return 0.0;

  const double expected = static_cast<double>(total) / static_cast<double>(m);
  double chi2 = 0.0;
  for (const auto c : counts) {
    const double diff = static_cast<double>(c) - expected;
    chi2 += diff * diff / expected;
  }
  return chi2;
}

double NormalCDF(double z)
{
  return 0.5 * std::erfc(-z / std::sqrt(2.0));
}

double ChiSquarePValue(double chi2, std::size_t df)
{
  if (df == 0) return 1.0;
  const double d = static_cast<double>(df);
  const double z = (std::pow(chi2 / d, 1.0 / 3.0) - (1.0 - 2.0 / (9.0 * d)))
                 / std::sqrt(2.0 / (9.0 * d));
  return 1.0 - NormalCDF(z);
}

static void RunOne(std::size_t L, double p, std::size_t bins, std::size_t df)
{
  // --- 1 trial ---
  percolation::data::PercolationSystem system1(L, p, 0xABCDEF01u);
  const auto counts1 = CountBins(system1.Matrix(), bins);
  const double chi2_1 = ChiSquare(counts1);
  const double pval_1 = ChiSquarePValue(chi2_1, df);
  const std::size_t total1 =
    std::accumulate(counts1.begin(), counts1.end(), std::size_t{0});
  const double pHat_1 = static_cast<double>(total1) / static_cast<double>(L * L);

  // --- TRIALS trials ---
  std::vector<std::size_t> idx(TRIALS);
  std::iota(idx.begin(), idx.end(), 0);
  std::vector<std::vector<std::size_t>> perTrial(TRIALS);

  std::for_each(std::execution::par, idx.begin(), idx.end(),
    [&](std::size_t t) {
      percolation::data::PercolationSystem system(
        L, p, static_cast<std::uint32_t>(0x1000u + t));
      perTrial[t] = CountBins(system.Matrix(), bins);
    });

  std::vector<std::size_t> totals(bins, 0);
  for (const auto & c : perTrial)
    for (std::size_t i = 0; i < bins; ++i)
      totals[i] += c[i];

  const double chi2_N = ChiSquare(totals);
  const double pval_N = ChiSquarePValue(chi2_N, df);
  const std::size_t totalN =
    std::accumulate(totals.begin(), totals.end(), std::size_t{0});
  const double pHat_N = static_cast<double>(totalN)
                      / static_cast<double>(TRIALS * L * L);

  // --- single combined row ---
  fmt::print(
    "{:>6} {:>4.0f} "
    "{:>10.2f} {:>8.4f} {:>5} {:>8.5f} "
    "{:>12.2f} {:>8.4f} {:>5} {:>8.5f}\n",
    L, p * 100.0,
    chi2_1, pval_1, (pval_1 < 0.05 ? "YES" : "no"), pHat_1,
    chi2_N, pval_N, (pval_N < 0.05 ? "YES" : "no"), pHat_N);
}

static void PrintHeader()
{
  fmt::print(
    "{:>6} {:>4} "
    "{:>10} {:>8} {:>5} {:>8} "
    "{:>12} {:>8} {:>5} {:>8}\n",
    "L", "p",
    "chi2(1)", "p(1)", "rej?", "phat(1)",
    "chi2(N)", "p(N)", "rej?", "phat(N)");
}

int main()
{
  for (const std::size_t L : LS) {
    const std::size_t bins = ScottBins(L * L);
    const std::size_t df   = bins - 1;

    fmt::print("{:->78}\n", "");
    fmt::print("L = {}, bins (modified Scott) = {}, df = {}\n",
               L, bins, df);

    PrintHeader();
    fmt::print("{:->78}\n", "");

    for (const double p : PS) {
      RunOne(L, p, bins, df);
    }
    fmt::print("\n");
  }

  return 0;
}