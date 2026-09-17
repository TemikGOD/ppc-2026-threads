#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <random>

#include "util/include/perf_test_util.hpp"
#include "denisov_a_qsort_simple_merge/common/include/common.hpp"
//#include "denisov_a_qsort_simple_merge/omp/include/ops_omp.hpp"
#include "denisov_a_qsort_simple_merge/seq/include/ops_seq.hpp"
//#include "denisov_a_qsort_simple_merge/stl/include/ops_stl.hpp"
//#include "denisov_a_qsort_simple_merge/tbb/include/ops_tbb.hpp"

namespace denisov_a_qsort_simple_merge {

class DenisovARunPerfTestsThreads : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_;

  void SetUp() override {
    constexpr size_t kCount = 100000;
    input_data_.resize(kCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-1000000, 1000000);

    for (size_t i = 0; i < kCount; ++i) {
      input_data_[i] = dist(gen);
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data.size() == input_data_.size() &&
             std::adjacent_find(output_data.begin(), output_data.end(),
                      [](const int &a, const int &b) { return a > b; }) == output_data.end();
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(DenisovARunPerfTestsThreads, RunPerfModes) {
  ExecuteTest(GetParam());
}

namespace {

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, YushkovaPHoareSortingSimpleMergingSEQ, YushkovaPHoareSortingSimpleMergingOMP,
                                YushkovaPHoareSortingSimpleMergingSTL, YushkovaPHoareSortingSimpleMergingTBB>(
        PPC_SETTINGS_denisov_a_qsort_simple_merge);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = DenisovARunPerfTestsThreads::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, DenisovARunPerfTestsThreads, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace denisov_a_qsort_simple_merge
