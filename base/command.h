// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_COMMAND_H
#define INSTALLER_BASE_COMMAND_H

#include <QStringList>

namespace installer {

// Run a script file in bash, no matter it is executable or not.
// First argument in |args| is the path to script file.
// Current working directory is changed to folder of |args[0]|.
// Returns true if |args[0]| executed and exited with 0.
// |output| and |err| are content of stdout and stderr.
bool RunScriptFile(const QStringList& args);
bool RunScriptFile(const QStringList& args, QString& output, QString& err);

// Run |cmd| with |args| in background and returns its result.
bool SpawnCmd(const QString& cmd, const QStringList& args);
bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output);
bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output,
              QString& err);

}  // namespace installer

#endif  // INSTALLER_BASE_COMMAND_H
