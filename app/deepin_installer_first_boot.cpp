// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Run this program to setup username, password, keyboard and timezone after
// reboot system. Only if `system_info_setup_after_reboot` flag is enabled.

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTranslator>

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "sysinfo/users.h"
#include "ui/delegates/language_delegate.h"
#include "ui/first_boot_setup_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Deepin Installer First Boot");
  app.setApplicationName("deepin-installer-first-boot");
  app.setApplicationVersion(installer::kAppVersion);
  app.setOrganizationDomain(installer::kOrganizationDomain);
  app.setWindowIcon(QIcon(":/images/whirlwind_installer.svg"));

  const char kLogFileName[] = "deepin-installer-first-boot.log";
  QString log_file;
  if (!installer::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";
    log_file = QString("/tmp/%1").arg(kLogFileName);
  } else {
    log_file = QString("/var/log/%1").arg(kLogFileName);
  }
  // Initialize log service.
  installer::RedirectLog(log_file);

  // Set language.
  QTranslator translator;
  const QString locale(installer::ReadLocale());
  translator.load(installer::GetLocalePath(locale));
  app.installTranslator(&translator);

  installer::FirstBootSetupWindow window;
  window.fullscreen();

  return app.exec();
}
