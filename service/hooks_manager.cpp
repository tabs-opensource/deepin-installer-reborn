// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/hooks_manager.h"

namespace service {

namespace {

}  // namespace

HooksManager::HooksManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("hooks_manager"));
}

}  // namespace service