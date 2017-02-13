// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/package_list.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(PackageListTest, GetPackageList) {
  const PackageList list = GetPackageList();
  EXPECT_EQ(list.length(), 2);
}

}  // namespace
}  // namespace installer