// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/package_list_model.h"

namespace installer {

PackageListModel::PackageListModel(QObject* parent)
    : QAbstractListModel(parent),
      packages_() {
  this->setObjectName("package_list_model");
}

QVariant PackageListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return packages_.at(index.row());
  } else {
    return QVariant();
  }
}

int PackageListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return packages_.length();
}

QString PackageListModel::getPackage(const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < packages_.length()) {
    return packages_.at(row);
  } else {
    return QString();
  }
}

QModelIndex PackageListModel::getPackageIndex(const QString& package) const {
  const int row = packages_.indexOf(package);
  if (row > -1) {
    return this->index(row);
  } else {
    return QModelIndex();
  }
}

void PackageListModel::setPackages(const QStringList& packages) {
  this->beginResetModel();
  packages_ = packages;
  this->endResetModel();
}

}  // namespace installer