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
