/*
 * sample_gtest_fixture.cpp
 *
 * Created on 12/4/23 9:26 PM
 * Description:
 *
 * Copyright (c) 2023 Ruixiang Du (rdu)
 */

#include "gtest/gtest.h"

#include "my_project/sample_class.hpp"

using namespace rxdu;

class SampleTestFixture : public ::testing::Test {
protected:
  SampleTestFixture() {
    // initialization code here
  }

  // put in any custom data members that you need
  rxdu::SampleClass sample_class;
};

TEST_F(SampleTestFixture, FixtureTest) { ASSERT_TRUE(sample_class.SayHello()); }