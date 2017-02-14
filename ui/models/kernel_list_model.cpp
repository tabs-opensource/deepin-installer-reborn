// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/kernel_list_model.h"

namespace installer {

KernelListModel::KernelListModel(QObject* parent)
    : QAbstractListModel(parent),
      package_list_(GetPackageList()) {
  this->setObjectName("kernel_list_model");
}

QVariant KernelListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return package_list_.at(index.row()).display_name;
  } else {
    return QVariant();
  }
}

int KernelListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return package_list_.length();
}

QString KernelListModel::getName(const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).name;
  } else {
    return QString();
  }
}

QStringList KernelListModel::getAvailablePackages(
    const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).available_packages;
  } else {
    return QStringList();
  }
}

QStringList KernelListModel::getSelectedPackages(
    const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    return package_list_.at(row).selected_packages;
  } else {
    return QStringList();
  }
}

void KernelListModel::setSelectedPackages(const QStringList& selected_packages,
                                           const QModelIndex& index) {
  const int row = index.row();
  if (index.isValid() && row < package_list_.length()) {
    package_list_[row].selected_packages = selected_packages;
  }
}

}  // namespace installer