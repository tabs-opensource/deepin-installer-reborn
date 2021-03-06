// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_partitions.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ParsePartitionItemsTest, ParsePartitionItems) {
  const PartitionItemList items = ParsePartitionItems();
  EXPECT_GT(items.length(), 0);
}

}  // namespace
}  // namespace installer