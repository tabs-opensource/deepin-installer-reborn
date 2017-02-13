// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_PACKAGE_LIST_H_
#define INSTALLER_SERVICE_PACKAGE_LIST_H_

#include <QList>
#include <QString>

namespace installer {

struct Package {
  QString name;
  QString display_name;
  QStringList available_packages;
  QStringList selected_packages;
};

typedef QList<Package> PackageList;

// Read package list from json file.
PackageList GetPackageList();

}  // namespace installer

#endif  // INSTALLER_SERVICE_PACKAGE_LIST_H_
