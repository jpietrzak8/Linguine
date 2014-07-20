//
// linvideodisplayform.cpp
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

#include "linvideodisplayform.h"
#include "ui_linvideodisplayform.h"

//#include "util/linfullscreenbutton.h"

//#include <QNetworkRequest>
//#include <QNetworkReply>
#include <gst/interfaces/xoverlay.h>
#include "lingstdatadialog.h"
#include <QMaemo5InformationBox>

#include <QDebug>

// Some GST defines:
#define GST_PLAY_FLAG_VIDEO 1
#define GST_PLAY_FLAG_AUDIO 2


// A C-style function to handle GST tag data:
static void linHandleTag(
  const GstTagList *list,
  const gchar *tag,
  gpointer linGstObjectPtr)
{
  LinVideoDisplayForm *myForm = 
    static_cast<LinVideoDisplayForm *>(linGstObjectPtr);

  gchar *value;

//  if (tag == GST_TAG_TITLE)
  if (strcmp(tag, GST_TAG_TITLE) == 0)
  {
    gst_tag_list_get_string(list, tag, &value);
    myForm->setTitle(QString(value));
    g_free(value);
  }
  else if (strcmp(tag, GST_TAG_ARTIST) == 0)
  {
    gst_tag_list_get_string(list, tag, &value);
    myForm->setArtist(QString(value));
    g_free(value);
  }
  else if (strcmp(tag, GST_TAG_ALBUM) == 0)
  {
    gst_tag_list_get_string(list, tag, &value);
    myForm->setAlbum(QString(value));
    g_free(value);
  }
}


// A C-style callback used to receive messages from GStreamer:
static gboolean linGstBusCallback(
  GstBus *bus,
  GstMessage *msg,
  gpointer linGstObjectPtr)
{
  Q_UNUSED(bus);

  switch (GST_MESSAGE_TYPE(msg))
  {
  case GST_MESSAGE_TAG:
    {
      GstTagList *tags = NULL;

      gst_message_parse_tag(msg, &tags);

      gst_tag_list_foreach(tags, linHandleTag, linGstObjectPtr);

      if (tags) gst_tag_list_free(tags);
    }
    break;

  case GST_MESSAGE_BUFFERING:
    {
      LinVideoDisplayForm *myForm =
        static_cast<LinVideoDisplayForm *>(linGstObjectPtr);

      gint percent = 0;
      gst_message_parse_buffering (msg, &percent);
//qDebug() << "Buffering percentage at " << percent;
      if (percent < 100)
      {
        myForm->waitForBuffer();
      }
      else
      {
        myForm->finishedBuffer();
      }
    }
    break;

  case GST_MESSAGE_EOS:
    {
      LinVideoDisplayForm *myForm =
        static_cast<LinVideoDisplayForm *>(linGstObjectPtr);

      myForm->stopPlaying();
    }
    break;

  case GST_MESSAGE_ERROR:
    {
      gchar *debug;
      GError *err;

      gst_message_parse_error(msg, &err, &debug);
      QString errString = "[";
      errString += GST_OBJECT_NAME(msg->src);
      errString += "]: ";
      errString += err->message;
      errString += " ";
      errString += debug;
      // Display the error message!
qDebug() << errString;

      LinVideoDisplayForm *myForm =
        static_cast<LinVideoDisplayForm *>(linGstObjectPtr);

      myForm->stopPlaying();
      
      g_free (debug);
      g_error_free (err);
    }
    break;

  default:
    break;
  }

  return true;
}


// Now, on to the actual methods:

LinVideoDisplayForm::LinVideoDisplayForm(
  QWidget *parent)
  : QWidget(parent),
    ui(new Ui::LinVideoDisplayForm),
//    fullScreenButton(0),
    xvsink(0),
    runningElement(0),
    gstreamerInUse(false),
    paused(true),
    waitingForBuffer(false),
    dataDialog(0)
{
  ui->setupUi(this);

//  ui->fullscreenButton->setIcon(QIcon::fromTheme("general_fullsize"));

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

//  fullScreenButton = new LinFullScreenButton(this);

  gst_init (NULL, NULL);

  dataDialog = new LinGstDataDialog(this);

  // Stick to the horizontal mode for now:
  ui->stackedWidget->setCurrentWidget(ui->horizontalPage);

  connect(
    &timer,
    SIGNAL(timeout()),
    this,
    SLOT(updateProgress()));
}


LinVideoDisplayForm::~LinVideoDisplayForm()
{
  stopPlaying();

//  if (fullScreenButton) delete fullScreenButton;
  if (dataDialog) delete dataDialog;

  delete ui;
}


void LinVideoDisplayForm::setProgram(
  QString title,
  QString mediaUrl)
{
  if (gstreamerInUse) stopPlaying();

  ui->hTitleLabel->setText(title);
  ui->vTitleLabel->setText(title);

//  GstElement *xvsink = gst_element_factory_make("xvimagesink", "xvsink");
  xvsink = gst_element_factory_make("xvimagesink", "xvsink");

  if (!xvsink)
  {
    QString err("Unable to create GStreamer element 'xvimagesink'");
    QMaemo5InformationBox::information(this, err);
    qDebug() << err;
    return;
  }

//qDebug() << "winId: " << ui->videoWidget->winId();
  unsigned long windowId = ui->hVideoWidget->winId();
  QApplication::syncX();
  gst_x_overlay_set_xwindow_id (
    GST_X_OVERLAY(G_OBJECT(xvsink)),
    windowId);
  gst_element_set_state(xvsink, GST_STATE_READY);

  g_object_set(
    G_OBJECT(xvsink),
    "force_aspect_ratio",
    true,
    NULL);

  GstElement *player = gst_element_factory_make("playbin2", "player");

  if (!player)
  {
    QString err("Unable to create GStreamer element 'playbin2'");
    QMaemo5InformationBox::information(this, err);
    qDebug() << err;
    return;
  }

  // Set up player for video and audio use:
  gint flags;
  g_object_get(G_OBJECT(player), "flags", &flags, NULL);
  flags |= GST_PLAY_FLAG_AUDIO | GST_PLAY_FLAG_VIDEO;
  g_object_set(G_OBJECT(player), "flags", flags, NULL);

  g_object_set(G_OBJECT(player), "video-sink", xvsink, NULL);

  QByteArray ba = mediaUrl.toAscii();
  g_object_set(G_OBJECT(player), "uri", ba.data(), NULL);

  runningElement = player;

  GstBus *bus = gst_element_get_bus(GST_ELEMENT(runningElement));
  gst_bus_add_watch(bus, linGstBusCallback, this);
  gst_object_unref(bus);

//  gst_element_set_state(runningElement, GST_STATE_PAUSED);
  gst_element_set_state(runningElement, GST_STATE_PLAYING);
  setPaused(false);

  ui->hPlayButton->setEnabled(true);
  ui->vPlayButton->setEnabled(true);

  gstreamerInUse = true;
}


void LinVideoDisplayForm::pausePlaying()
{
  if (!runningElement) return;

  if (paused)
  {
    gst_element_set_state(runningElement, GST_STATE_PLAYING);
    setPaused(false);
  }
  else
  {
    gst_element_set_state(runningElement, GST_STATE_PAUSED);
    setPaused(true);
  }
}


void LinVideoDisplayForm::stopPlaying()
{
  if (!runningElement) return;

  gst_element_set_state(runningElement, GST_STATE_NULL);

  gst_object_unref(GST_OBJECT(runningElement));

  runningElement = 0;

  gstreamerInUse = false;

  ui->hPlayButton->setEnabled(false);
  ui->vPlayButton->setEnabled(false);
  setPaused(true);
}


void LinVideoDisplayForm::waitForBuffer()
{
  if (!runningElement) return;

  if (!waitingForBuffer)
  {
    gst_element_set_state(runningElement, GST_STATE_PAUSED);
    waitingForBuffer = true;
  }
}


void LinVideoDisplayForm::resizeEvent(
  QResizeEvent *event)
{
  QWidget::resizeEvent(event);

  // Need to switch the video to the appropriate page here!
  if (ui->stackedWidget->width() > ui->stackedWidget->height())
  {
    ui->stackedWidget->setCurrentWidget(ui->horizontalPage);
  }
  else
  {
    ui->stackedWidget->setCurrentWidget(ui->verticalPage);
  }
}


void LinVideoDisplayForm::closeEvent(
  QCloseEvent *event)
{
  stopPlaying();

  // Set landscape orientation to false:
  parentWidget()->setAttribute(static_cast<Qt::WidgetAttribute>(129), false);
  // Set auto orientation to true:
  parentWidget()->setAttribute(static_cast<Qt::WidgetAttribute>(130), true);

  QWidget::closeEvent(event);
}


void LinVideoDisplayForm::finishedBuffer()
{
  if (!runningElement) return;

  if (waitingForBuffer)
  {
    if (!paused)
    {
      gst_element_set_state(runningElement, GST_STATE_PLAYING);
    }

    waitingForBuffer = false;
  }
}


/*
void LinVideoDisplayForm::enterFullscreen()
{
  gst_element_set_state(runningElement, GST_STATE_PAUSED);
  // Hide everything but the video:
  ui->titleLabel->hide();
  ui->playPauseButton->hide();
  ui->stopButton->hide();
//  ui->fullscreenButton->hide();

  showFullScreen();
//  QApplication::syncX();
  gst_element_set_state(runningElement, GST_STATE_PLAYING);
}
*/


/*
void LinVideoDisplayForm::returnFromFullscreen()
{
  gst_element_set_state(runningElement, GST_STATE_PAUSED);
  // Make everything visible again:
  ui->titleLabel->show();
  ui->playPauseButton->show();
  ui->stopButton->show();
//  ui->fullscreenButton->show();

  showMaximized();
//  QApplication::syncX();
  gst_element_set_state(runningElement, GST_STATE_PLAYING);
}
*/


/*
void LinVideoDisplayForm::on_fullscreenButton_clicked()
{
  enterFullscreen();
}
*/

void LinVideoDisplayForm::on_hPlayButton_clicked()
{
  pausePlaying();
}

void LinVideoDisplayForm::on_hStopButton_clicked()
{
  stopPlaying();
}

void LinVideoDisplayForm::on_hInfoButton_clicked()
{
  dataDialog->displayData(runningElement);
}

void LinVideoDisplayForm::on_vPlayButton_clicked()
{
  pausePlaying();
}

void LinVideoDisplayForm::on_vStopButton_clicked()
{
  stopPlaying();
}

void LinVideoDisplayForm::on_vInfoButton_clicked()
{
  dataDialog->displayData(runningElement);
}


void LinVideoDisplayForm::setTitle(
  QString title)
{
  dataDialog->setTitle(title);
}


void LinVideoDisplayForm::setArtist(
  QString artist)
{
  dataDialog->setArtist(artist);
}


void LinVideoDisplayForm::setAlbum(
  QString album)
{
  dataDialog->setAlbum(album);
}


void LinVideoDisplayForm::setPaused(
  bool p)
{
  paused = p;

  if (paused)
  {
    ui->hPlayButton->setIcon(QIcon(":/icons/playback_play_icon&48.png"));
    ui->vPlayButton->setIcon(QIcon(":/icons/playback_play_icon&48.png"));
    timer.stop();
  }
  else
  {
    ui->hPlayButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
    ui->vPlayButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
    timer.start(1000);
  }
}


void LinVideoDisplayForm::updateProgress()
{
  GstQuery *query;

  // Determine the duration of the stream:
  gint64 duration = 0;
  gint64 position = 0;

  query = gst_query_new_duration(GST_FORMAT_TIME);

  if (gst_element_query(runningElement, query))
  {
    gst_query_parse_duration(query, NULL, &duration);
  }

  gst_query_unref(query);

  if (duration == 0)
  {
    // Duration not available; no point in continuing any further.
    return;
  }

  // Determine the position of the stream:

  query = gst_query_new_position(GST_FORMAT_TIME);

  if (gst_element_query(runningElement, query))
  {
    gst_query_parse_position(query, NULL, &position);
  }

  gst_query_unref(query);

  // Determine the percentage:
  int percentage = (position * 100) / duration;

  ui->vProgressBar->setValue(percentage);
  QString percentString;
  percentString.setNum(percentage);
  percentString.append("%");
  ui->hPercentageLabel->setText(percentString);
}
