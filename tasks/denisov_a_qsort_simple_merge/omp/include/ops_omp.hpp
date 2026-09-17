#pragma once

#include <vector>

#include "task/include/task.hpp"
#include "denisov_a_qsort_simple_merge/common/include/common.hpp"

namespace denisov_a_qsort_simple_merge {

class DenisovAQsortSimpleMergeOMP : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kOMP;
  }
  explicit DenisovAQsortSimpleMergeOMP(const InType &in);

 private:
  static int HoarePartition(std::vector<int> &data, int left, int right);
  static void HoareSort(std::vector<int> &data, int left, int right);
  static void SimpleMerge(std::vector<int> &data, int left, int mid, int right);

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace denisov_a_qsort_simple_merge
