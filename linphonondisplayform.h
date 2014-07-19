//
// linphonodisplayform.h
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

#ifndef LINPHONONDISPLAYFORM_H
#define LINPHONONDISPLAYFORM_H

#include <QWidget>

class LinFullScreenButton;

namespace Ui {
class LinPhononDisplayForm;
}

class LinPhononDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinPhononDisplayForm(QWidget *parent = 0);
  ~LinPhononDisplayForm();

public slots:
  void setProgram(
    QString title,
    QString mediaUrl);

  void pausePlaying();

  void stopPlaying();

  void enterFullscreen();
  void returnFromFullscreen();

private slots:
  void on_playPauseButton_clicked();
  void on_stopButton_clicked();
  void on_fullscreenButton_clicked();
  
private:
  Ui::LinPhononDisplayForm *ui;

  LinFullScreenButton *fullScreenButton;

  bool paused;
};

#endif // LINPHONONDISPLAYFORM_H
