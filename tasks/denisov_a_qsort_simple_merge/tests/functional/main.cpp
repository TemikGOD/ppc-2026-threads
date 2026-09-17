#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"
#include "denisov_a_qsort_simple_merge/common/include/common.hpp"
#include "denisov_a_qsort_simple_merge/omp/include/ops_omp.hpp"
#include "denisov_a_qsort_simple_merge/seq/include/ops_seq.hpp"
//#include "denisov_a_qsort_simple_merge/stl/include/ops_stl.hpp"
#include "denisov_a_qsort_simple_merge/tbb/include/ops_tbb.hpp"

namespace denisov_a_qsort_simple_merge {

class DenisovARunFuncTestsThreads : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return std::get<1>(test_param) + "_n" + std::to_string(std::get<0>(test_param).size());
  }

 protected:
  void SetUp() override {
    const TestType &params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    input_data_ = std::get<0>(params);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    if (output_data.size() != input_data_.size()) {
      return false;
    }
    if (!IsSorted(output_data)) {
      return false;
    }

    std::vector<int> expected = input_data_;
    std::sort(expected.begin(), expected.end());
    return output_data == expected;
  }

  [[nodiscard]] static bool IsSorted(const OutType &data) {
    return std::is_sorted(data.begin(), data.end());
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
};

namespace {

TEST_P(DenisovARunFuncTestsThreads, HoareSortSimpleMergingSEQ) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 9> kTestParam = {
    std::make_tuple(std::vector<int>{67}, "single"),
    std::make_tuple(std::vector<int>{22, 21}, "two_elements"),
    std::make_tuple(std::vector<int>{15, 26, 37, 48, 59}, "already_sorted"),
    std::make_tuple(std::vector<int>{55, 46, 37, 28, 19}, "reverse_sorted"),
    std::make_tuple(std::vector<int>{8, 2, 8, 2, 8, 2}, "duplicates"),
    std::make_tuple(std::vector<int>{0, -7, 7, -4, 4, -3}, "mixed_signs"),
    std::make_tuple(std::vector<int>{100, 1, 50, 2, 75, 3, 60, 4, 20, 5, 30}, "odd_count"),
    std::make_tuple(std::vector<int>{9, 9, 8, 8, 7, 7, 6, 6, 5, 5}, "pair_duplicates"),
    std::make_tuple(std::vector<int>{1000, -1000, 500, -500, 0, 250, -250}, "wide_range")};

const auto kTestTasksList = std::tuple_cat(ppc::util::AddFuncTask<DenisovAQsortSimpleMergeSEQ, InType>(
                                               kTestParam, PPC_SETTINGS_denisov_a_qsort_simple_merge),
                                           ppc::util::AddFuncTask<DenisovAQsortSimpleMergeOMP, InType>(
                                               kTestParam, PPC_SETTINGS_denisov_a_qsort_simple_merge),
                                           ppc::util::AddFuncTask<DenisovAQsortSimpleMergeTBB, InType>(
                                               kTestParam, PPC_SETTINGS_denisov_a_qsort_simple_merge));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = DenisovARunFuncTestsThreads::PrintFuncTestName<DenisovARunFuncTestsThreads>;

INSTANTIATE_TEST_SUITE_P(HoareSortSimpleMergingTests, DenisovARunFuncTestsThreads, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace denisov_a_qsort_simple_merge
