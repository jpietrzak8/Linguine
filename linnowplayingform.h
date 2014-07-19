//
// linnowplayingform.h
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

#ifndef LINNOWPLAYINGFORM_H
#define LINNOWPLAYINGFORM_H

#include <QWidget>

#include <QString>
#include <gst/gst.h>
#include <QPixmap>
#include <QTimer>

class LinGstDataDialog;

namespace Ui {
class LinNowPlayingForm;
}

class LinNowPlayingForm : public QWidget
{
  Q_OBJECT

public:
  explicit LinNowPlayingForm(QWidget *parent = 0);
  ~LinNowPlayingForm();

  void setTitle(
    QString title);

  void setArtist(
    QString artist);

  void setAlbum(
    QString album);

public slots:
  void setProgram(
    QPixmap image,
    QString title,
    QString mediaUrl);

  void pausePlaying();

  void stopPlaying();

protected:
  void resizeEvent(
    QResizeEvent *event);
  
private slots:
  void on_vPlayButton_clicked();
  void on_vStopButton_clicked();
  void on_vInfoButton_clicked();
  void on_hPlayButton_clicked();
  void on_hStopButton_clicked();
  void on_hInfoButton_clicked();

  void updateProgress();

private:
//  void displayAudioTags();
  void displayImage();
  void setPaused(
    bool p);

  Ui::LinNowPlayingForm *ui;

  GstElement *runningElement;

  bool gstreamerInUse;
  bool paused;

  QPixmap currentImage;
  LinGstDataDialog *dataDialog;
  QTimer timer;
};

#endif // LINNOWPLAYINGFORM_H
