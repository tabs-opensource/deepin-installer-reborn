// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_kernel_frame.h"

#include <QEvent>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/models/kernel_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

SelectKernelFrame::SelectKernelFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("select_kernel_frame");

  this->initUI();
  this->initConnections();
}

void SelectKernelFrame::changeEvent(QEvent* event) {
  QFrame::changeEvent(event);
}

void SelectKernelFrame::initConnections() {
  connect(kernel_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &SelectKernelFrame::onVersionViewSelectionChanged);

  connect(next_button_, &QPushButton::clicked,
          this, &SelectKernelFrame::finished);
}

void SelectKernelFrame::initUI() {
  title_label_ = new TitleLabel("Select Kernel");

  kernel_view_ = new FramelessListView();
  kernel_view_->setObjectName("kernel_view");
  kernel_view_->setFixedWidth(340);
  version_model_ = new KernelListModel();
  kernel_view_->setModel(version_model_);

  next_button_ = new NavButton(tr("Next"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(kernel_view_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(next_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
}

void SelectKernelFrame::onVersionViewSelectionChanged(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  const QString name = version_model_->getName(current);
  const QStringList selected_packs =
      version_model_->getSelectedPackages(current);
  const QStringList avail_packs =
      version_model_->getAvailablePackages(current);

//  // Update package list and selected packages.
//  package_model_->setPackages(avail_packs);
//
//  QItemSelection selected;
//  for (const QString& package : selected_packs) {
//    const QModelIndex index = package_model_->getPackageIndex(package);
//    selected.select(index, index);
//    package_model_->getPackageIndex(package);
//  }
//
//  // Block signals when update selections.
//  package_view_->blockSignals(true);
//  package_view_->selectionModel()->select(selected,
//                                          QItemSelectionModel::Select);
//  package_view_->blockSignals(false);
}

}  // namespace installer