// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
#define INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_

#include <QFrame>
class QListView;

namespace installer {

class NavButton;
class VersionListModel;

class PackageListFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PackageListFrame(QWidget* parent = nullptr);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* next_button_ = nullptr;
  QListView* version_view_ = nullptr;
  VersionListModel* version_model_ = nullptr;
  QListView* packages_view_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
