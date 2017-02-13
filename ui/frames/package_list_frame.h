// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
#define INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_

#include <QFrame>
#include <QtCore/QItemSelection>

class QListView;

namespace installer {

class NavButton;
class PackageListModel;
class VersionListModel;

// To display packages to be installed into target system.
class PackageListFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PackageListFrame(QWidget* parent = nullptr);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  QListView* version_view_ = nullptr;
  VersionListModel* version_model_ = nullptr;
  QListView* package_view_ = nullptr;
  PackageListModel* package_model_ = nullptr;

  NavButton* next_button_ = nullptr;

 private slots:
  // Update package list in package_view_ when new package is selected in
  // version_view_.
  void onVersionViewSelectionChanged(const QModelIndex& current,
                                     const QModelIndex& previous);

  // Update selected package list in version_view_ when package selection state
  // is updated in package_view_.
  void onPackageViewSelectionChanged(const QItemSelection& selected,
                                     const QItemSelection& deselected);

  // Save current package selection state when next_button_ is clicked.
  void onNextButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
