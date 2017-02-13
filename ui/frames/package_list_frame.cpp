// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/package_list_frame.h"

#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/models/package_list_model.h"
#include "ui/models/version_list_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PackageListFrame::PackageListFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("package_list_frame");

  this->initUI();
  this->initConnections();
}

void PackageListFrame::initConnections() {
  connect(version_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &PackageListFrame::onVersionViewSelectionChanged);


  connect(next_button_, &QPushButton::clicked,
          this, &PackageListFrame::finished);
}

void PackageListFrame::initUI() {
  TitleLabel* title_label = new TitleLabel("Select Packages");
  CommentLabel* comment_label = new CommentLabel("Select packages to install");
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label);

  version_view_ = new FramelessListView();
  version_view_->setObjectName("version_view");
  version_view_->setFixedWidth(340);
  version_model_ = new VersionListModel();
  version_view_->setModel(version_model_);

  package_view_ = new FramelessListView();
  package_view_->setObjectName("package_view");
  package_view_->setFixedWidth(519);
  package_model_ = new PackageListModel();
  package_view_->setModel(package_model_);
  // Enable multi-selection.
  package_view_->setSelectionMode(QAbstractItemView::MultiSelection);

  QHBoxLayout* package_layout = new QHBoxLayout();
  package_layout->setContentsMargins(0, 0, 0, 0);
  package_layout->setSpacing(0);
  package_layout->addStretch();
  package_layout->addWidget(version_view_);
  // Add 1px margin between these two list views.
  package_layout->addSpacing(1);
  package_layout->addWidget(package_view_);
  package_layout->addStretch();

  QFrame* package_frame = new QFrame();
  package_frame->setObjectName("package_frame");
  package_frame->setContentsMargins(0, 0, 0, 0);
  package_frame->setLayout(package_layout);
  package_frame->setFixedWidth(860);
  QSizePolicy package_size_policy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  package_size_policy.setVerticalStretch(1);
  package_frame->setSizePolicy(package_size_policy);

  next_button_ = new NavButton("Next");

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignHCenter);
  layout->addLayout(comment_layout, 0);
  layout->addStretch();
  layout->addWidget(package_frame, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(next_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/package_list_frame.css"));
}

void PackageListFrame::onVersionViewSelectionChanged(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  const QString name = version_model_->getName(current);
  const QStringList selected_packs =
      version_model_->getSelectedPackages(current);
  const QStringList avail_packs =
      version_model_->getAvailablePackages(current);
  package_model_->setPackage(avail_packs);
}

}  // namespace installer