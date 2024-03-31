//Copyright 2024 Petrov Maksim

#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/petrov_m_radix_sort_double_simple_merging/include/ops_seq.hpp"

TEST(Petrov_M_Radix_Sort_Sequential1, test_pipeline_run) {
  std::vector<double> in(1000000);
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<double> distr(-10000, 100000);
  for (auto &num : in) {
    num = distr(eng);
  }
  std::vector<double> sorted = in;
  std::sort(sorted.begin(), sorted.end());
  std::vector<double> out(in.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential = std::make_shared<RadixSortDoubleSequential>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(sorted[i], out[i]);
  }
}

TEST(Petrov_M_Radix_Sort_Sequential1, test_task_run) {
  std::vector<double> in(1000000);
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<double> distr(-10000, 10000);
  for (auto &num : in) {
    num = distr(eng);
  }
  std::vector<double> sorted = in;
  std::sort(sorted.begin(), sorted.end());
  std::vector<double> out(in.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential = std::make_shared<RadixSortDoubleSequential>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(sorted[i], out[i]);
  }
}
