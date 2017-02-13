// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/package_list_frame.h"

#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
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

  connect(package_view_->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this, &PackageListFrame::onPackageViewSelectionChanged);

  connect(next_button_, &QPushButton::clicked,
          this, &PackageListFrame::onNextButtonClicked);
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

  // Update package list and selected packages.
  package_model_->setPackages(avail_packs);

  QItemSelection selected;
  for (const QString& package : selected_packs) {
    const QModelIndex index = package_model_->getPackageIndex(package);
    selected.select(index, index);
    package_model_->getPackageIndex(package);
  }

  // Block signals when update selections.
  package_view_->blockSignals(true);
  package_view_->selectionModel()->select(selected,
                                          QItemSelectionModel::Select);
  package_view_->blockSignals(false);
}

void PackageListFrame::onPackageViewSelectionChanged(
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

  const QModelIndex index = version_view_->selectionModel()->currentIndex();
  version_model_->setSelectedPackages(selected_packages, index);
}

void PackageListFrame::onNextButtonClicked() {
  // Read all selected packages.
  const QModelIndexList selected_indexes =
      package_view_->selectionModel()->selectedIndexes();
  QStringList selected_packages;
  for (const QModelIndex& index : selected_indexes) {
    selected_packages.append(package_model_->getPackage(index));
  }

  const QModelIndex index = version_view_->selectionModel()->currentIndex();
  const QString name =version_model_->getName(index);

  WriteSelectedPackageList(name, selected_packages);

  emit this->finished();
}

}  // namespace installer