// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNNER_SYSTEM_INFO_KEYBOARD_FRAME_H
#define INSTALLER_UI_FRAMES_INNNER_SYSTEM_INFO_KEYBOARD_FRAME_H

#include <QFrame>
class QLineEdit;

namespace installer {

class CommentLabel;
class FramelessListView;
class KeyboardLayoutModel;
class KeyboardLayoutVariantModel;
class NavButton;
class TitleLabel;

// Keyboard layout setup page.
class SystemInfoKeyboardFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoKeyboardFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when back_button_ is clicked.
  void finished();

  // Emitted when new keyboard layout is selected.
  void layoutUpdated(const QString& layout);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  FramelessListView* layout_view_ = nullptr;
  KeyboardLayoutModel* layout_model_ = nullptr;
  FramelessListView* variant_view_ = nullptr;
  KeyboardLayoutVariantModel* variant_model_ = nullptr;
  QLineEdit* test_edit_ = nullptr;
  NavButton* back_button_ = nullptr;

 private slots:
  // Update variant list when new keyboard layout is selected.
  // Update system keyboard layout.
  // Emit layoutUpdate() signal.
  // Clear content of test_edit_.
  void onLayoutViewSelected(const QModelIndex& index);

  // Update system keyboard layout when new layout variant is selected.
  // Clear content of test_edit_.
  void onVariantViewSelected(const QModelIndex& index);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNNER_SYSTEM_INFO_KEYBOARD_FRAME_H