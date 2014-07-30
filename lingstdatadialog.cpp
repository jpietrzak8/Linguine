//
// lingstdatadialog.cpp
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

#include "lingstdatadialog.h"
#include "ui_lingstdatadialog.h"

#include <QString>
#include <QTextStream>


LinGstDataDialog::LinGstDataDialog(
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::LinGstDataDialog),
    duration(0)
{
  ui->setupUi(this);
}


LinGstDataDialog::~LinGstDataDialog()
{
  delete ui;
}


void LinGstDataDialog::retrieveDuration(
  GstElement *pipeline)
{
  GstQuery *query;
  int seconds = 0;
  int minutes = 0;
  int hours = 0;
  QString outputString;
  QTextStream qts(&outputString);

  // Determine the duration of the stream:

  query = gst_query_new_duration(GST_FORMAT_TIME);

  if (gst_element_query(pipeline, query))
  {
    gst_query_parse_duration(query, NULL, &duration);

    gint64 displayVal = duration;

    // displayVal is in nanoseconds.  Divide by 1000000000 to get seconds.
    displayVal /= 1000000000;
    seconds = displayVal % 60;

    displayVal /= 60;
    minutes = displayVal % 60;

    hours = displayVal / 60;
  }

  gst_query_unref(query);

  if (hours < 10) qts << "0";
  qts << hours << ":";
  if (minutes < 10) qts << "0";
  qts << minutes << ":";
  if (seconds < 10) qts << "0";
  qts << seconds;

  ui->durationLabel->setText(outputString);
  outputString.clear();
}


gint64 LinGstDataDialog::retrievePosition(
  GstElement *pipeline)
{
  GstQuery *query;
  gint64 position;
  int seconds = 0;
  int minutes = 0;
  int hours = 0;
  QString outputString;
  QTextStream qts(&outputString);

  // Determine the position of the stream:

  query = gst_query_new_position(GST_FORMAT_TIME);

  if (gst_element_query(pipeline, query))
  {
    gst_query_parse_position(query, NULL, &position);
    // Position is in nanoseconds.  Divide by 1000000000 to get seconds.
    position /= 1000000000;
    seconds = position % 60;

    position /= 60;
    minutes = position % 60;

    hours = position / 60;
  }

  gst_query_unref(query);

  if (hours < 10) qts << "0";
  qts << hours << ":";
  if (minutes < 10) qts << "0";
  qts << minutes << ":";
  if (seconds < 10) qts << "0";
  qts << seconds;
  ui->positionLabel->setText(outputString);
  outputString.clear();

  return position;
}


int LinGstDataDialog::displayData(
  GstElement *pipeline)
{
  retrievePosition(pipeline);

  return exec();
}


void LinGstDataDialog::setTitle(
  QString title)
{
  ui->titleLabel->setText(title);
}


void LinGstDataDialog::setArtist(
  QString artist)
{
  ui->artistLabel->setText(artist);
}


void LinGstDataDialog::setAlbum(
  QString album)
{
  ui->albumLabel->setText(album);
}


void LinGstDataDialog::reset()
{
  duration = 0;

  ui->durationLabel->setText("unknown");
  ui->positionLabel->setText("unknown");
  ui->titleLabel->setText("unknown");
  ui->artistLabel->setText("unknown");
  ui->albumLabel->setText("unknown");
}
