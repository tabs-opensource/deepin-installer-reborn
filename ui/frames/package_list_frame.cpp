// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/package_list_frame.h"

#include <QVBoxLayout>

#include "ui/frames/consts.h"
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
  version_model_ = new VersionListModel();
  version_view_->setModel(version_model_);

  next_button_ = new NavButton("Next");

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignHCenter);
  layout->addLayout(comment_layout, 0);
  layout->addStretch();
  layout->addWidget(version_view_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(next_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
}

}  // namespace installer