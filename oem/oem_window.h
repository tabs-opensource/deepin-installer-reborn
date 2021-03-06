// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_OEM_OEM_WINDOW_H
#define INSTALLER_OEM_OEM_WINDOW_H

#include <QFrame>
class QListView;

namespace installer {

class OemSettingsItemView;
class OemSettingsModel;

// Main window of deepin-installer-oem tool.
class OemWindow : public QFrame {
  Q_OBJECT

 public:
  explicit OemWindow(QWidget* parent = nullptr);

  // Initialize internal data.
  bool init();

 private:
  void initConnections();
  void initUI();

  OemSettingsModel* model_ = nullptr;
  QListView* name_list_view_ = nullptr;
  OemSettingsItemView* item_view_ = nullptr;

 private slots:
  // Update item in item viewer when it is selected.
  void onNameListViewSelected(const QModelIndex& current,
                              const QModelIndex& previous);
};

}  // namespace installer

#endif  // INSTALLER_OEM_OEM_WINDOW_H