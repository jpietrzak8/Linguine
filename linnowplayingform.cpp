//
// linnowplayingform.cpp
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

#include "linnowplayingform.h"
#include "ui_linnowplayingform.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMaemo5InformationBox>
#include "lingstdatadialog.h"

#include <QDebug>

// Some GST defines:
#define GST_PLAY_FLAG_VIDEO 1
#define GST_PLAY_FLAG_AUDIO 2
#define GST_PLAY_FLAG_VIS   8


// A C-style function to handle GST tag data:
static void linHandleTag(
  const GstTagList *list,
  const gchar *tag,
  gpointer linGstObjectPtr)
{
  LinNowPlayingForm *myForm = 
    static_cast<LinNowPlayingForm *>(linGstObjectPtr);

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

  case GST_MESSAGE_EOS:
    {
      LinNowPlayingForm *myForm =
        static_cast<LinNowPlayingForm *>(linGstObjectPtr);

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

      LinNowPlayingForm *myForm =
        static_cast<LinNowPlayingForm *>(linGstObjectPtr);

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

LinNowPlayingForm::LinNowPlayingForm(
  QWidget *parent)
  : QWidget(parent),
    ui(new Ui::LinNowPlayingForm),
    runningElement(0),
    gstreamerInUse(false),
    paused(true),
    dataDialog(0)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  gst_init (NULL, NULL);
//  gst_version(&major, &minor, &micro, &nano);

  dataDialog = new LinGstDataDialog(this);

  connect(
    &timer,
    SIGNAL(timeout()),
    this,
    SLOT(updateProgress()));
}


LinNowPlayingForm::~LinNowPlayingForm()
{
  stopPlaying();

  if (dataDialog) delete dataDialog;

  delete ui;
}


void LinNowPlayingForm::setProgram(
  QPixmap image,
  QString title,
  QString mediaUrl)
{
  if (gstreamerInUse) stopPlaying();

  currentImage = image;
  displayImage();

  ui->vNameLabel->setText(title);
  ui->hNameLabel->setText(title);

  GstElement *player = gst_element_factory_make("playbin2", "player");

  if (!player)
  {
    QString err("Unable to create GStreamer element 'playbin2'");
    QMaemo5InformationBox::information(this, err);
    qDebug() << err;
    return;
  }

  // Set up player for audio-only use:
  gint flags;
  g_object_get(G_OBJECT(player), "flags", &flags, NULL);
  flags |= GST_PLAY_FLAG_AUDIO;
  flags &= ~(GST_PLAY_FLAG_VIDEO | GST_PLAY_FLAG_VIS);
  g_object_set(G_OBJECT(player), "flags", flags, NULL);

  // Define the URI for the player:
  QByteArray ba = mediaUrl.toAscii();
  g_object_set(G_OBJECT(player), "uri", ba.data(), NULL);

  runningElement = player;

/*
  runningElement = gst_pipeline_new("pipe");

  if (!runningElement)
  {
qDebug() << "Unable to create GStreamer pipeline";
//    throw something("Unable to create GStreamer pipeline");
    return;
  }

  gst_bin_add_many(
    GST_BIN(runningElement),
    player,
    NULL);
*/

  GstBus *bus = gst_element_get_bus(GST_ELEMENT(runningElement));
  gst_bus_add_watch(bus, linGstBusCallback, this);
  gst_object_unref(bus);

//  gst_element_set_state(runningElement, GST_STATE_PAUSED);
//  paused = true;
  gst_element_set_state(runningElement, GST_STATE_PLAYING);
  setPaused(false);

  gstreamerInUse = true;

  ui->vPlayButton->setEnabled(true);
  ui->hPlayButton->setEnabled(true);
}


void LinNowPlayingForm::pausePlaying()
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


void LinNowPlayingForm::stopPlaying()
{
  if (!runningElement) return;

  gst_element_set_state(runningElement, GST_STATE_NULL);

  gst_object_unref(GST_OBJECT(runningElement));

  runningElement = 0;

  gstreamerInUse = false;

  ui->vPlayButton->setEnabled(false);
  ui->hPlayButton->setEnabled(false);
  setPaused(true); // Kind of awkward, but this stops the timer.
}


void LinNowPlayingForm::resizeEvent(
  QResizeEvent *event)
{
  QWidget::resizeEvent(event);

  if (ui->stackedWidget->width() > ui->stackedWidget->height())
  {
    ui->stackedWidget->setCurrentWidget(ui->horizontalPage);
  }
  else
  {
    ui->stackedWidget->setCurrentWidget(ui->verticalPage);
  }

  displayImage();
}


void LinNowPlayingForm::closeEvent(
  QCloseEvent *event)
{
  stopPlaying();

  QWidget::closeEvent(event);
}


void LinNowPlayingForm::on_vPlayButton_clicked()
{
  pausePlaying();
}

void LinNowPlayingForm::on_vStopButton_clicked()
{
  stopPlaying();
}

void LinNowPlayingForm::on_vInfoButton_clicked()
{
  dataDialog->displayData(runningElement);
}

void LinNowPlayingForm::on_hPlayButton_clicked()
{
  pausePlaying();
}

void LinNowPlayingForm::on_hStopButton_clicked()
{
  stopPlaying();
}

void LinNowPlayingForm::on_hInfoButton_clicked()
{
  dataDialog->displayData(runningElement);
}


/*
void LinNowPlayingForm::displayAudioTags()
{
  QString dataString;
  QTextStream qts(&dataString, QIODevice::WriteOnly | QIODevice::Text);

  gint numAudioStreams;
  g_object_get(G_OBJECT(runningElement), "n-audio", numAudioStreams, NULL);
  qts << "Number of audio streams: " << numAudioStreams << "\n";

  int index = 0;
  GstTagList *tags = 0;
  gchar *serializedTags = 0;
  while (index < numAudioStreams)
  {
    g_signal_emit_by_name(
      G_OBJECT(runningElement),
      "get_audio_tags",
      index,
      &tags);

    qts << "Tags for audio stream #" << index << ":\n";

    serializedTags = gst_tag_list_to_string(tags);
    qts << serializedTags << "\n";

    g_free(serializedTags);
    serializedTags = 0;
    gst_tag_list_free(tags);
    tags = 0;

    ++index;
  }

  dataDialog->setText(dataString);

  dataDialog->exec();
}
*/


void LinNowPlayingForm::displayImage()
{
  if (ui->stackedWidget->currentWidget() == ui->verticalPage)
  {
    ui->vImageLabel->setPixmap(
      currentImage.scaled(
        ui->vImageLabel->height(),
        ui->vImageLabel->width(),
        Qt::KeepAspectRatio));
  }
  else
  {
    ui->hImageLabel->setPixmap(
      currentImage.scaled(
        ui->hImageLabel->width(),
        ui->hImageLabel->height(),
        Qt::KeepAspectRatio));
  }
}


void LinNowPlayingForm::setTitle(
  QString title)
{
  dataDialog->setTitle(title);
}


void LinNowPlayingForm::setArtist(
  QString artist)
{
  dataDialog->setArtist(artist);
}


void LinNowPlayingForm::setAlbum(
  QString album)
{
  dataDialog->setAlbum(album);
}


void LinNowPlayingForm::setPaused(
  bool p)
{
  paused = p;

  if (paused)
  {
    ui->vPlayButton->setIcon(QIcon(":/icons/playback_play_icon&48.png"));
    ui->hPlayButton->setIcon(QIcon(":/icons/playback_play_icon&48.png"));
    timer.stop();
  }
  else
  {
    ui->vPlayButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
    ui->hPlayButton->setIcon(QIcon(":/icons/playback_pause_icon&48.png"));
    timer.start(1000);
  }
}


void LinNowPlayingForm::updateProgress()
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
