/*
 * sample_gtest.cpp
 *
 * Created on 12/4/23 9:24 PM
 * Description:
 *
 * Copyright (c) 2023 Ruixiang Du (rdu)
 */

#include "gtest/gtest.h"

#include "my_project/sample_class.hpp"

using namespace rxdu;

TEST(SampleTest, SimpleTest) {
  rxdu::SampleClass sample_class;

  ASSERT_TRUE(sample_class.SayHello());
}
