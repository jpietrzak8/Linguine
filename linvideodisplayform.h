//
// linvideodisplayform.h
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

#ifndef LINVIDEODISPLAYFORM_H
#define LINVIDEODISPLAYFORM_H

#include <QWidget>

#include <QString>
#include <gst/gst.h>
#include <QTimer>

//class LinFullScreenButton;
class LinGstDataDialog;
class QResizeEvent;
class QCloseEvent;

namespace Ui {
class LinVideoDisplayForm;
}

class LinVideoDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  LinVideoDisplayForm(
    QWidget *parent);

  ~LinVideoDisplayForm();

  void waitForBuffer();
  void finishedBuffer();

  void setTitle(
    QString title);

  void setArtist(
    QString artist);

  void setAlbum(
    QString album);

  bool gstElementQuery(
    GstQuery *query);

  bool gstObjectMatches(
    GstObject *obj);

  void setSeeking(
    bool seekingEnabled);

  bool seekingUnknown() { return !checkedSeeking; }

  void setupDataDialog();

public slots:
  void setProgram(
    QString feedName,
    QString summary,
    QString pubDate,
    QString mediaUrl);

  void pausePlaying();

  void stopPlaying();

//  void enterFullscreen();
//  void returnFromFullscreen();

protected:
//  void resizeEvent(
//    QResizeEvent *event);

  void closeEvent(
    QCloseEvent *event);

private slots:
//  void on_fullscreenButton_clicked();
  void on_playButton_clicked();
//  void on_seekButton_clicked();
//  void on_stopButton_clicked();
  void on_infoButton_clicked();
//  void on_openUIButton_clicked();
//  void on_closeUIButton_clicked();
  void on_seekSlider_sliderMoved(
    int percentage);

  void updateProgress();

private:
  void setPaused(
    bool p);

  void setDND(
    bool dndFlag);

  Ui::LinVideoDisplayForm *ui;

//  LinFullScreenButton *fullScreenButton;

//  QWidget *videoWidget;

  GstElement *xvsink;
  GstElement *runningElement;

  bool gstreamerInUse;
  bool paused;
  bool waitingForBuffer;
  bool checkedSeeking;

  LinGstDataDialog *dataDialog;
  QTimer timer;
};

#endif // LINVIDEODISPLAYFORM_H
