/*
 * test_sample.cpp
 *
 * Created on 12/4/23 9:16 PM
 * Description:
 *
 * Copyright (c) 2023 Ruixiang Du (rdu)
 */

#include <iostream>

#include "my_project/sample_class.hpp"
#include "version.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Library " << PROJECT_NAME
            << " version: " << PROJECT_VERSION_STRING << std::endl;

  rxdu::SampleClass sample_class;
  sample_class.SayHello();

  return 0;
}