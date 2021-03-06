// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hook_worker.h"

#include <QDebug>

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Absolute path to hook_manager.sh
const char kHookManagerFile[] = BUILTIN_HOOKS_DIR "/hook_manager.sh";

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  const QStringList args = {kHookManagerFile, hook};
  return RunScriptFile(args);
}

}  // namespace

HookWorker::HookWorker(QObject* parent) : QObject(parent) {
  this->setObjectName("hook_worker");
  connect(this, &HookWorker::runHook,
          this, &HookWorker::handleRunHook);
}

void HookWorker::handleRunHook(const QString& hook) {
  const bool ok = RunHook(hook);
  emit this->hookFinished(ok);
}

}  // namespace installer