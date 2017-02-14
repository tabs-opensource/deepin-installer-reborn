// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_kernel_frame.h"

#include <QEvent>
#include <QVBoxLayout>

#include "service/settings_manager.h"
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
  kernel_model_ = new KernelListModel();
  kernel_view_->setModel(kernel_model_);
  QSizePolicy list_policy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
  kernel_view_->setSizePolicy(list_policy);

  // Get title name from json file.
  title_label_->setText(kernel_model_->getTitle());
  // Select default item.

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

  // Read and save kernel version.
  const QString kernel_version = kernel_model_->getKernelVersion(current);
  WriteKernelVersion(kernel_version);
}

}  // namespace installer