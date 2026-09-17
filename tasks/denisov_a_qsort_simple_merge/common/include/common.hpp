#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace denisov_a_qsort_simple_merge {

using InType = std::vector<int>;
using OutType = std::vector<int>;
using TestType = std::tuple<std::vector<int>, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace denisov_a_qsort_simple_merge
