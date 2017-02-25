// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/log_manager.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>

namespace installer {

namespace {

// Application-wide log filepath.
QString g_log_file;

void BackupLogFile() {
  QFile file(g_log_file);
  if (file.exists()) {
    const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    const QString new_path = QString("%1.%2").arg(g_log_file).arg(timestamp);
    file.rename(new_path);
  }
}

#ifndef N_DEBUG
// Customized QtMessageHandler.
void InstallerMessageOutput(QtMsgType msg_type,
                            const QMessageLogContext& context,
                            const QString& msg) {
  const QByteArray& msg_ref(msg.toLocal8Bit());
  const QString filename = QFileInfo(context.file).fileName();
  const QByteArray& filename_ref(filename.toLocal8Bit());
  switch (msg_type) {
    case QtDebugMsg: {
      fprintf(stdout, "[Debug: %s:%u] %s\n",
              filename_ref.constData(), context.line, msg_ref.constData());
      fflush(stdout);
      break;
    }
    case QtInfoMsg: {
      fprintf(stdout, "[Info: %s:%u] %s\n",
              filename_ref.constData(), context.line, msg_ref.constData());
      fflush(stdout);
      break;
    }
    case QtWarningMsg: {
      fprintf(stderr, "[Warning: %s:%u] %s\n",
              filename_ref.constData(), context.line, msg_ref.constData());
      fflush(stderr);
      break;
    }
    case QtCriticalMsg: {
      fprintf(stderr, "[Critical: %s:%u] %s\n",
              filename_ref.constData(), context.line, msg_ref.constData());
      fflush(stderr);
      break;
    }
    case QtFatalMsg: {
      fprintf(stderr, "[Fatal: %s:%u] %s\n",
              filename_ref.constData(), context.line, msg_ref.constData());
      fflush(stderr);
      // Abort process.
      abort();
    }
  }
}
#endif

}  // namespace

QString GetLogFilepath() {
  return g_log_file;
}

bool RedirectLog(const QString& log_file) {
  // Store log filepath and backup old log file.
  g_log_file = log_file;
  BackupLogFile();

#ifndef N_DEBUG
  // Customize qt message in debug mode.
  qInstallMessageHandler(InstallerMessageOutput);
#endif

  int log_fd = open(log_file.toStdString().c_str(), O_RDWR | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (log_fd == -1) {
    qCritical() << "Failed to create log file:" << log_file;
    return false;
  }
  bool ok = true;
  if (dup2(log_fd, STDOUT_FILENO) == -1) {
    qCritical() << "Failed to redirect stdout";
    ok = false;
  }
  if (dup2(log_fd, STDERR_FILENO) == -1) {
    qCritical() << "Failed to redirect stderr";
    ok = false;
  }
  return ok;
}

}  // namespace installer
