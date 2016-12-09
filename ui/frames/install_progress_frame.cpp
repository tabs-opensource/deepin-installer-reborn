// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_progress_frame.h"

#include <QDebug>
#include <QEvent>
#include <QProgressBar>
#include <QStyle>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/hooks_manager.h"
#include "ui/frames/consts.h"
#include "ui/frames/inner/install_progress_slide_frame.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/install_progress_tip.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kProgressBarWidth = 640;
const int kTooltipWidth = 60;
const int kTooltipHeight = 31;
const int kTooltipFrameWidth = kProgressBarWidth + kTooltipWidth;

const int kRetainingInterval = 3000;

const char kTextTitle[] = "Installing";
const char kTextComment[] = "You will be experiencing the incredible pleasant "
    "of deepin after the time for just a cup of coffee";

}  // namespace

InstallProgressFrame::InstallProgressFrame(QWidget* parent)
    : QFrame(parent),
      failed_(true),
      hooks_manager_(new HooksManager()),
      hooks_manager_thread_(new QThread()),
      retaining_timer_(new QTimer(this)) {
  this->setObjectName("install_progress_frame");

  hooks_manager_->moveToThread(hooks_manager_thread_);
  hooks_manager_thread_->start();

  retaining_timer_->setInterval(kRetainingInterval);

  this->initUI();
  this->initConnections();
}

InstallProgressFrame::~InstallProgressFrame() {
  delete hooks_manager_;
  hooks_manager_ = nullptr;

  hooks_manager_thread_->quit();
  hooks_manager_thread_->wait();
  delete hooks_manager_thread_;
  hooks_manager_thread_ = nullptr;
}

void InstallProgressFrame::startSlide(bool position_animation,
                                      bool opacity_animation) {
  slide_frame_->startSlide(position_animation, opacity_animation);
}

void InstallProgressFrame::runHooks(bool ok) {
  qDebug() << "runHooks()" << ok;

  if (ok) {
    // Partition operations take 5% progress.
    this->onProgressUpdate(kBeforeChrootStartVal);

    qDebug() << "emit runHooks() signal";
    // Run hooks/ in background thread.
    emit hooks_manager_->runHooks();
  } else {
    this->onHooksErrorOccurred();
  }
}

void InstallProgressFrame::updateLanguage(const QString& locale) {
  slide_frame_->setLocale(locale);
}

void InstallProgressFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
  } else {
    QFrame::changeEvent(event);
  }
}

void InstallProgressFrame::initConnections() {
  connect(hooks_manager_, &HooksManager::errorOccurred,
          this, &InstallProgressFrame::onHooksErrorOccurred);
  connect(hooks_manager_, &HooksManager::finished,
          this, &InstallProgressFrame::onHooksFinished);
  connect(hooks_manager_, &HooksManager::processUpdate,
          this, &InstallProgressFrame::onProgressUpdate);

  connect(retaining_timer_, &QTimer::timeout,
          this, &InstallProgressFrame::onRetainingTimerTimeout);
}

void InstallProgressFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  slide_frame_ = new InstallProgressSlideFrame();

  QFrame* tooltip_frame = new QFrame();
  tooltip_frame->setObjectName("tooltip_frame");
  tooltip_frame->setContentsMargins(0, 0, 0, 0);
  tooltip_frame->setFixedSize(kTooltipFrameWidth, kTooltipHeight);
  tooltip_label_ = new InstallProgressTip(tooltip_frame);
  tooltip_label_->setFixedSize(kTooltipWidth, kTooltipHeight);
  tooltip_label_->setAlignment(Qt::AlignHCenter);
  tooltip_label_->setText("0%");

  progress_bar_ = new QProgressBar();
  progress_bar_->setObjectName("progress_bar");
  progress_bar_->setFixedSize(kProgressBarWidth, 8);
  progress_bar_->setTextVisible(false);
  progress_bar_->setRange(0, 100);
  progress_bar_->setOrientation(Qt::Horizontal);
  progress_bar_->setValue(0);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(slide_frame_, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(tooltip_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(5);
  layout->addWidget(progress_bar_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/install_progress_frame.css"));
}

void InstallProgressFrame::onHooksErrorOccurred() {
  qDebug() << "onHooksErrorOccurred()";
  failed_ = true;
  slide_frame_->stopSlide();
  retaining_timer_->stop();
  emit this->finished();
}

void InstallProgressFrame::onHooksFinished() {
  failed_ = false;

  // Set progress value to 100 explicitly.
  this->onProgressUpdate(100);

  retaining_timer_->start();
}

void InstallProgressFrame::onProgressUpdate(int progress) {
  progress_bar_->setValue(progress);
  tooltip_label_->setText(QString("%1%").arg(progress));
  const int x = kProgressBarWidth * progress / 100;
  const int y = tooltip_label_->y();
  tooltip_label_->move(x, y);

  // Force QProgressBar to repaint.
  this->style()->unpolish(progress_bar_);
  this->style()->polish(progress_bar_);
  progress_bar_->repaint();
}

void InstallProgressFrame::onRetainingTimerTimeout() {
  qDebug() << "onRetainingTimerTimeout()";
  slide_frame_->stopSlide();
  retaining_timer_->stop();
  emit this->finished();
}

}  // namespace installer