#pragma once

#include <vector>

#include "denisov_a_qsort_simple_merge/common/include/common.hpp"
#include "task/include/task.hpp"

namespace denisov_a_qsort_simple_merge {

class DenisovAQsortSimpleMergeSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit DenisovAQsortSimpleMergeSEQ(const InType &in);

 private:
  static void HoareSort(std::vector<int> &data, int left, int right);
  static int HoarePartition(std::vector<int> &data, int left, int right);
  static std::vector<int> SimpleMerge(const std::vector<int> &left, const std::vector<int> &right);

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace denisov_a_qsort_simple_merge
