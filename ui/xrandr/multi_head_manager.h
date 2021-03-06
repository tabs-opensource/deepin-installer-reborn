// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H
#define INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H

#include <QObject>
#include <QRect>
#include <QVector>

class QThread;

namespace installer {

class MultiHeadWorker;
class WallpaperItem;

// Manage wallpapers of multi-head system.
class MultiHeadManager : public QObject {
  Q_OBJECT

 public:
  explicit MultiHeadManager(QObject* parent = nullptr);
  ~MultiHeadManager();

 public slots:
  // Switch display mode.
  void switchXRandRMode();

  // Update wallpaper items background current screens information.
  void updateWallpaper();

 signals:
  // Emitted when primary screen changed to |geometry|.
  void primaryScreenChanged(const QRect& geometry);

  // Emit this signal to notify xrandr to switch to mirror mode.
  void switchToMirrorMode();

 private:
  void initConnections();

  QVector<WallpaperItem*> wallpaper_items_;
  QThread* multi_head_thread_ = nullptr;
  MultiHeadWorker* multi_head_worker_ = nullptr;

  // Number of monitors defined in last xrandr output.
  // This values changes when a new monitor is connected to or an existing
  // monitor is disconnected from system.
  // If so, switch to mirror mode if possible.
  int last_connected_monitors_;

  // Geometry of primary output used at last.
  QRect last_primary_geometry_;

 private slots:
  // Repaint background when output added, changed or removed.
  void onMonitorsChanged();

  void doSwitchToMirrorMode();
};

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H
