#include "lingstdatadialog.h"
#include "ui_lingstdatadialog.h"

#include <QString>
#include <QTextStream>


LinGstDataDialog::LinGstDataDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LinGstDataDialog)
{
  ui->setupUi(this);
}

LinGstDataDialog::~LinGstDataDialog()
{
  delete ui;
}

int LinGstDataDialog::displayData(
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
    gint64 duration;
    gst_query_parse_duration(query, NULL, &duration);
    // Duration is in nanoseconds.  Divide by 1000000000 to get seconds.
    duration /= 1000000000;
    seconds = duration % 60;

    duration /= 60;
    minutes = duration % 60;

    hours = duration / 60;
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

  // Determine the position of the stream:

  query = gst_query_new_position(GST_FORMAT_TIME);

  if (gst_element_query(pipeline, query))
  {
    gint64 position;
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
