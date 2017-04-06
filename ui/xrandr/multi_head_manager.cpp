// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/xrandr/multi_head_manager.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QThread>

#include "ui/widgets/wallpaper_item.h"
#include "ui/xrandr/xrandr_wrapper.h"

namespace installer {

MultiHeadManager::MultiHeadManager(QObject* parent)
    : QObject(parent),
      wallpaper_items_() {
  this->setObjectName("wallpaper_manager");
}

MultiHeadManager::~MultiHeadManager() {
}

void MultiHeadManager::switchXRandRMode() {
}

void MultiHeadManager::updateWallpaper() {
  qDebug() << "MultiHeadManager::updateWallpaper()";
  // Clear old wallpaper items
  for (WallpaperItem* item : wallpaper_items_) {
    delete item;
    item = nullptr;
  }
  wallpaper_items_.clear();

  const QRect geometry = qApp->desktop()->screenGeometry();
  WallpaperItem* item = new WallpaperItem(geometry);
  wallpaper_items_.append(item);
  item->show();

  // Notify main window to change geometry
  emit this->primaryScreenChanged(geometry);
}

}  // namespace installer