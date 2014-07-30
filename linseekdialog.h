#ifndef LINSEEKDIALOG_H
#define LINSEEKDIALOG_H

#include <QDialog>

namespace Ui {
class LinSeekDialog;
}

class LinSeekDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinSeekDialog(
    QWidget *parent = 0);

  ~LinSeekDialog();

  void setupAndExec(
    int percentagePlayed);

  void reset();

signals:
  void seekToPercentage(
    int percentage);
  
private slots:
/*
  void on_seekSlider_valueChanged(
    int value);
*/

  void on_seekSlider_sliderMoved(
    int value);

private:
  Ui::LinSeekDialog *ui;
};

#endif // LINSEEKDIALOG_H
