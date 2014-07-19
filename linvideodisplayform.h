#ifndef LINVIDEODISPLAYFORM_H
#define LINVIDEODISPLAYFORM_H

#include <QWidget>

#include <QString>
#include <gst/gst.h>
#include <QTimer>

//class LinFullScreenButton;
class LinGstDataDialog;

namespace Ui {
class LinVideoDisplayForm;
}

class LinVideoDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinVideoDisplayForm(QWidget *parent = 0);
  ~LinVideoDisplayForm();

  void waitForBuffer();
  void finishedBuffer();

  void setTitle(
    QString title);

  void setArtist(
    QString artist);

  void setAlbum(
    QString album);

public slots:
  void setProgram(
    QString title,
    QString mediaUrl);

  void pausePlaying();

  void stopPlaying();

//  void enterFullscreen();
//  void returnFromFullscreen();

protected:
  void resizeEvent(
    QResizeEvent *event);

private slots:
//  void on_fullscreenButton_clicked();
  void on_hPlayButton_clicked();
  void on_hStopButton_clicked();
  void on_hInfoButton_clicked();
  void on_vPlayButton_clicked();
  void on_vStopButton_clicked();
  void on_vInfoButton_clicked();

  void updateProgress();

private:
  void setPaused(
    bool p);

  Ui::LinVideoDisplayForm *ui;

//  LinFullScreenButton *fullScreenButton;

  GstElement *xvsink;
  GstElement *runningElement;

  bool gstreamerInUse;
  bool paused;
  bool waitingForBuffer;

  LinGstDataDialog *dataDialog;
  QTimer timer;
};

#endif // LINVIDEODISPLAYFORM_H
