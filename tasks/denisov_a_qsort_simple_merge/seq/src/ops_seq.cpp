#include "denisov_a_qsort_simple_merge/seq/include/ops_seq.hpp"

#include <cstddef>
#include <random>
#include <stack>
#include <utility>
#include <vector>

#include "denisov_a_qsort_simple_merge/common/include/common.hpp"

namespace denisov_a_qsort_simple_merge {

DenisovAQsortSimpleMergeSEQ::DenisovAQsortSimpleMergeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().clear();
}

bool DenisovAQsortSimpleMergeSEQ::ValidationImpl() {
  return !(GetInput().empty());
}

bool DenisovAQsortSimpleMergeSEQ::PreProcessingImpl() {
  GetOutput() = GetInput();
  return true;
}

void DenisovAQsortSimpleMergeSEQ::HoareSort(std::vector<int> &data, int left, int right) {
  if (data.empty()) {
    return;
  }
  
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

int DenisovAQsortSimpleMergeSEQ::HoarePartition(std::vector<int> &data, int left, int right) {
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

std::vector<int> DenisovAQsortSimpleMergeSEQ::SimpleMerge(const std::vector<int> &left, const std::vector<int> &right) {
  std::vector<int> result;
  result.reserve(left.size() + right.size());

  size_t left_index = 0;
  size_t right_index = 0;

  while (left_index < left.size() && right_index < right.size()) {
    if (left[left_index] <= right[right_index]) {
      result.push_back(left[left_index]);
      ++left_index;
    } else {
      result.push_back(right[right_index]);
      ++right_index;
    }
  }

  while (left_index < left.size()) {
    result.push_back(left[left_index]);
    ++left_index;
  }

  while (right_index < right.size()) {
    result.push_back(right[right_index]);
    ++right_index;
  }

  return result;
}

bool DenisovAQsortSimpleMergeSEQ::RunImpl() {
  std::vector<int>& data = GetOutput();
  if (data.size() <= 1) {
    return true;
  }

  size_t mid = data.size() / 2;
  std::vector<int> left(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(mid));
  std::vector<int> right(data.begin() + static_cast<std::ptrdiff_t>(mid), data.end());

  if (left.size() > 1) {
    HoareSort(left, 0, static_cast<int>(left.size()) - 1);
  }

  if (right.size() > 1) {
    HoareSort(right, 0, static_cast<int>(right.size()) - 1);
  }

  data = SimpleMerge(left, right);
  
  return true;
}

bool DenisovAQsortSimpleMergeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace denisov_a_qsort_simple_merge
