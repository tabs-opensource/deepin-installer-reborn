// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/version_list_model.h"

namespace installer {

VersionListModel::VersionListModel(QObject* parent)
    : QAbstractListModel(parent),
      package_list_(GetPackageList()) {
  this->setObjectName("version_list_model");
}

QVariant VersionListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return package_list_.at(index.row()).display_name;
  } else {
    return QVariant();
  }
}

int VersionListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return package_list_.length();
}

QString VersionListModel::getName(const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).name;
  } else {
    return QString();
  }
}

QStringList VersionListModel::getAvailablePackages(
    const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).available_packages;
  } else {
    return QStringList();
  }
}

QStringList VersionListModel::getSelectedPackages(
    const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).selected_packages;
  } else {
    return QStringList();
  }
}

void VersionListModel::setSelectedPackages(const QStringList& selected_packages,
                                           const QModelIndex& index) {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    package_list_[row].selected_packages = selected_packages;
  }
}

}  // namespace installer