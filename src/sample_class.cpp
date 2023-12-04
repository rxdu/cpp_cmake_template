/*
 * sample_class.cpp
 *
 * Created on 12/4/23 9:16 PM
 * Description:
 *
 * Copyright (c) 2023 Ruixiang Du (rdu)
 */

#include "my_project/sample_class.hpp"

#include <iostream>

namespace rxdu {
bool SampleClass::SayHello() {
  std::cout << "Hello World!" << std::endl;
  return true;
}
} // namespace rxdu