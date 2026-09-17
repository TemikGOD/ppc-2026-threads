#include "denisov_a_qsort_simple_merge/omp/include/ops_omp.hpp"

#include <omp.h>

#include <algorithm>
#include <cstddef>
#include <stack>
#include <utility>
#include <random>
#include <vector>

#include "denisov_a_qsort_simple_merge/common/include/common.hpp"

namespace denisov_a_qsort_simple_merge {

DenisovAQsortSimpleMergeOMP::DenisovAQsortSimpleMergeOMP(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().clear();
}

int DenisovAQsortSimpleMergeOMP::HoarePartition(std::vector<int> &data, int left, int right) {
  static thread_local std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(left, right);
  int pivot = data[dist(gen)];

  int i = left;
  int j = right;

  while (true) {
    while (data[i] < pivot) {
      ++i;
    }

    while (data[j] > pivot) {
      --j;
    }

    if (i >= j) {
      return j;
    }

    std::swap(data[i], data[j]);
    ++i;
    --j;
  }
}

void DenisovAQsortSimpleMergeOMP::HoareSort(std::vector<int> &data, int left, int right) {
  std::stack<std::pair<int, int>> ranges;
  ranges.emplace(left, right);

  while (!ranges.empty()) {
    auto [l, r] = ranges.top();
    ranges.pop();

    if (l >= r) {
      continue;
    }

    int pivot_index = HoarePartition(data, l, r);
    if ((pivot_index - l) > (r - (pivot_index + 1))) {
      ranges.emplace(l, pivot_index);
      ranges.emplace(pivot_index + 1, r);
    } else {
      ranges.emplace(pivot_index + 1, r);
      ranges.emplace(l, pivot_index);
    }
  }
}

void DenisovAQsortSimpleMergeOMP::SimpleMerge(std::vector<int> &data, int left, int mid, int right) {
  std::vector<int> merged;
  const int merged_size = (right - left) + 1;
  merged.reserve(static_cast<std::size_t>(merged_size));

  int left_index = left;
  int right_index = mid + 1;

  while (left_index <= mid && right_index <= right) {
    if (data[left_index] <= data[right_index]) {
      merged.push_back(data[left_index++]);
    } else {
      merged.push_back(data[right_index++]);
    }
  }

  while (left_index <= mid) {
    merged.push_back(data[left_index++]);
  }

  while (right_index <= right) {
    merged.push_back(data[right_index++]);
  }

  for (std::size_t idx = 0; idx < merged.size(); ++idx) {
    data[static_cast<std::size_t>(left) + idx] = merged[idx];
  }
}

bool DenisovAQsortSimpleMergeOMP::ValidationImpl() {
  return !GetInput().empty();
}

bool DenisovAQsortSimpleMergeOMP::PreProcessingImpl() {
  GetOutput() = GetInput();
  return true;
}

bool DenisovAQsortSimpleMergeOMP::RunImpl() {
  std::vector<int> &data = GetOutput();
  const int n = static_cast<int>(data.size());
  if (n <= 1) {
    return true;
  }

  const int max_threads = std::max(1, omp_get_max_threads());
  const int chunks = std::min(max_threads, n);

  if (chunks == 1) {
    HoareSort(data, 0, n - 1);
    return true;
  }

  std::vector<int> borders(static_cast<std::size_t>(chunks + 1));
  for (int i = 0; i <= chunks; ++i) {
    borders[static_cast<std::size_t>(i)] = (i * n) / chunks;
  }

#pragma omp parallel for default(none) shared(data, borders, chunks)
  for (int chunk = 0; chunk < chunks; ++chunk) {
    const int left = borders[static_cast<std::size_t>(chunk)];
    const int right = borders[static_cast<std::size_t>(chunk) + 1] - 1;
    if (left < right) {
      HoareSort(data, left, right);
    }
  }

  for (int width = 1; width < chunks; width *= 2) {
    const int merge_count = (chunks + (2 * width) - 1) / (2 * width);

#pragma omp parallel for default(none) shared(data, borders, chunks, width, merge_count)
    for (int merge = 0; merge < merge_count; ++merge) {
      const int left_chunk = 2 * merge * width;
      const int mid_chunk = left_chunk + width;
      const int right_chunk = std::min(left_chunk + (2 * width), chunks);

      if (mid_chunk < right_chunk) {
        const int left = borders[static_cast<std::size_t>(left_chunk)];
        const int mid = borders[static_cast<std::size_t>(mid_chunk)] - 1;
        const int right = borders[static_cast<std::size_t>(right_chunk)] - 1;
        SimpleMerge(data, left, mid, right);
      }
    }
  }

  return true;
}

bool DenisovAQsortSimpleMergeOMP::PostProcessingImpl() {
    return !GetOutput().empty();
}

}  // namespace denisov_a_qsort_simple_merge
