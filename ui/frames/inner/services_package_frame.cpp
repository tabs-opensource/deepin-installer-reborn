// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/services_package_frame.h"

#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/models/package_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

ServicesPackageFrame::ServicesPackageFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("services_package_frame");

  this->initUI();
  this->initConnections();
}

void ServicesPackageFrame::initConnections() {
  connect(package_view_->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this, &ServicesPackageFrame::onPackageViewSelectionChanged);

  connect(previous_button_, &QPushButton::clicked,
          this, &ServicesPackageFrame::goPrevious);
  connect(next_button_, &QPushButton::clicked,
          this, &ServicesPackageFrame::finished);
}

void ServicesPackageFrame::initUI() {
  title_label_ = new TitleLabel("");

  package_view_ = new FramelessListView();
  package_view_->setObjectName("package_view");
  package_view_->setFixedWidth(519);
  package_model_ = new PackageListModel();
  package_view_->setModel(package_model_);
  // Enable multi-selection.
  package_view_->setSelectionMode(QAbstractItemView::MultiSelection);

  previous_button_ = new NavButton(tr("Previous"));
  next_button_ = new NavButton(tr("Next"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(previous_button_, 0, Qt::AlignHCenter);
  layout->addSpacing(30);
  layout->addWidget(next_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
}


void ServicesPackageFrame::onPackageViewSelectionChanged(
    const QItemSelection& selected, const QItemSelection& deselected) {
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  // Read all selected packages.
  const QModelIndexList selected_indexes =
      package_view_->selectionModel()->selectedIndexes();
  QStringList selected_packages;
  for (const QModelIndex& index : selected_indexes) {
    selected_packages.append(package_model_->getPackage(index));
  }

//  const QModelIndex index = kernel_view_->selectionModel()->currentIndex();
//  version_model_->setSelectedPackages(selected_packages, index);
}

}  // namespace installer