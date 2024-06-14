// Copyright 2024 Bonyuk Peter
#include "tbb/bonyuk_p_gauss_gorizont/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <cmath>
#include <random>
#include <thread>

std::vector<int> getImage(int n, int m, uint8_t min, uint8_t max) {
  int size = n * m;
  std::random_device device;
  std::mt19937 mt(device());
  std::uniform_int_distribution<int> distribut(min, max);
  std::vector<int> image(size);
  for (int i = 0; i < size; i++) {
    image[i] = static_cast<int>(distribut(mt));
  }
  return image;
}

bool LinearGaussianFiltering::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<int *>(taskData->inputs[0])[0];
  width = reinterpret_cast<int *>(taskData->inputs[0])[1];
  min = reinterpret_cast<int *>(taskData->inputs[2])[0];
  max = reinterpret_cast<int *>(taskData->inputs[2])[1];
  for (int i = 0; i < height * width; i++) {
    input.push_back(reinterpret_cast<int *>(taskData->inputs[1])[i]);
    res.push_back(0);
  }
  return true;
}

bool LinearGaussianFiltering::validation() {
  internal_order_test();
  bool flag = false;
  if (taskData->inputs_count[1] == taskData->outputs_count[0]) flag = true;
  return flag;
}

bool LinearGaussianFiltering::run() {
  internal_order_test();
  std::vector<int> filteredImage(input.size(), 0);
  std::vector<int> gauss_kernel = {1, 2, 1, 2, 4, 2, 1, 2, 1};
  int kSize = 3;
  int sum;
  tbb::parallel_for(0, height, 1, [&](int i) {
    for (int j = 0; j < width; ++j) {
      sum = 0;
      for (int m = 0; m < kSize; ++m) {
        for (int n = 0; n < kSize; ++n) {
          int row = i + m - kSize / 2;
          int col = j + n - kSize / 2;
          if (row >= 0 && row < height && col >= 0 && col < width) {
            sum += getPixel(row, col) * gauss_kernel[m * kSize + n];
          }
        }
      }
      sum = std::min(sum, 255);
      setPixel(i, j, sum);
    }
  });
  return true;
}

bool LinearGaussianFiltering::post_processing() {
  internal_order_test();
  for (int i = 0; i < height * width; ++i) {
    reinterpret_cast<int *>(taskData->outputs[0])[i] = res[i];
  }
  return true;
}

int LinearGaussianFiltering::getPixel(int x, int y) { return input[x * width + y]; }

void LinearGaussianFiltering::setPixel(int x, int y, int value) { res[x * width + y] = value; }