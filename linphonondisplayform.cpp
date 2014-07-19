//
// linphonondisplayform.cpp
//
// Copyright 2014 by John Pietrzak (jpietrzak8@gmail.com)
//
// This file is part of Linguine.
//
// Linguine is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Linguine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Linguine; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "linphonondisplayform.h"
#include "ui_linphonondisplayform.h"

#include <Phonon/MediaSource>
#include "util/linfullscreenbutton.h"

LinPhononDisplayForm::LinPhononDisplayForm(
  QWidget *parent)
  : QWidget(parent),
    ui(new Ui::LinPhononDisplayForm),
    fullScreenButton(0),
    paused(false)
{
  ui->setupUi(this);

  ui->fullscreenButton->setIcon(QIcon::fromTheme("general_fullsize"));

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  fullScreenButton = new LinFullScreenButton(this);
}


LinPhononDisplayForm::~LinPhononDisplayForm()
{
  stopPlaying();

  if (fullScreenButton) delete fullScreenButton;

  delete ui;
}


void LinPhononDisplayForm::setProgram(
  QString title,
  QString mediaUrl)
{
  ui->titleLabel->setText(title);

  Phonon::MediaSource m(mediaUrl);

  ui->videoPlayer->play(m);

  ui->playPauseButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
  ui->playPauseButton->setEnabled(true);
}


void LinPhononDisplayForm::pausePlaying()
{
  if (paused)
  {
    ui->videoPlayer->play();
    ui->playPauseButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
    paused = false;
  }
  else
  {
    ui->videoPlayer->pause();
    ui->playPauseButton->setIcon(QIcon(":/icons/playback_play_icon&48.png"));
    paused = true;
  }
}


void LinPhononDisplayForm::stopPlaying()
{
  ui->videoPlayer->stop();
  ui->playPauseButton->setEnabled(false);
}


void LinPhononDisplayForm::enterFullscreen()
{
  ui->titleLabel->hide();
  ui->playPauseButton->hide();
  ui->stopButton->hide();
  ui->fullscreenButton->hide();

  showFullScreen();
}


void LinPhononDisplayForm::returnFromFullscreen()
{
  ui->titleLabel->show();
  ui->playPauseButton->show();
  ui->stopButton->show();
  ui->fullscreenButton->show();

  showMaximized();
}


void LinPhononDisplayForm::on_playPauseButton_clicked()
{
  pausePlaying();
}


void LinPhononDisplayForm::on_stopButton_clicked()
{
  stopPlaying();
}


void LinPhononDisplayForm::on_fullscreenButton_clicked()
{
  enterFullscreen();
}
