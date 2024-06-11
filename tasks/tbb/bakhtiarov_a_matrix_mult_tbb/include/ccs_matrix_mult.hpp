// Copyright 2024 Bakhtiarov Alexander
#pragma once

#include <memory>
#include <vector>

#include "core/task/include/task.hpp"

class SparseTBBMatrixMulti : public ppc::core::Task {
 public:
  explicit SparseTBBMatrixMulti(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool run() override = 0;
  bool post_processing() override;
  bool validation();

 private:
  struct Matrix {
    std::vector<double> values;
    std::vector<int> rows;
    std::vector<int> colPtr;
    int numRows;
    int numCols;
  };

 protected:
  Matrix matrix1;
  Matrix matrix2;
  Matrix result;

  void construct_ccs(Matrix& matrix, const double* data, int numRows, int numCols);
  double doubleFromBytes(uint8_t* buffer);
};

class SparseTBBMatrixMultiSequential : public SparseTBBMatrixMulti {
 public:
  explicit SparseTBBMatrixMultiSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : SparseTBBMatrixMulti(std::move(taskData_)) {}
  bool run() override;
};

class SparseTBBMatrixMultiParallel : public SparseTBBMatrixMulti {
 public:
  explicit SparseTBBMatrixMultiParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : SparseTBBMatrixMulti(std::move(taskData_)) {}
  bool run() override;
};
