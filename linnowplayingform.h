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
