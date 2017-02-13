// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/package_list.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace installer {

namespace {

const char kPackageFile[] = RESOURCES_DIR "/packages.json";

}  // namespace

PackageList GetPackageList() {
  PackageList list;

  const QString content(ReadFile(kPackageFile));
  if (content.isEmpty()) {
    qWarning() << "Failed to read package list file";
    return list;
  }

  const QJsonArray package_list =
      QJsonDocument::fromJson(content.toUtf8()).array();
  for (const QJsonValue& package_value : package_list) {
    const QJsonObject obj = package_value.toObject();
    Package package;
    package.name = obj.value("name").toString();
    package.display_name = obj.value("displayName").toString();

    const QJsonArray avail_packs = obj.value("availablePackages").toArray();
    for (const QJsonValue& avail_pack : avail_packs) {
      package.available_packages.append(avail_pack.toString());
    }

    const QJsonArray selected_packs = obj.value("selectedPackages").toArray();
    for (const QJsonValue& selected_pack : selected_packs) {
      package.selected_packages.append(selected_pack.toString());
    }

    list.append(package);
  }

  return list;
}

}  // namespace installer