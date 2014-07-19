#ifndef LINGSTDATADIALOG_H
#define LINGSTDATADIALOG_H

#include <QDialog>

#include <gst/gst.h>

namespace Ui {
class LinGstDataDialog;
}

class LinGstDataDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinGstDataDialog(QWidget *parent = 0);
  ~LinGstDataDialog();

  int displayData(
    GstElement *pipeline);

  void setTitle(
    QString title);

  void setArtist(
    QString artist);

  void setAlbum(
    QString album);

private:
  Ui::LinGstDataDialog *ui;
};

#endif // LINGSTDATADIALOG_H
