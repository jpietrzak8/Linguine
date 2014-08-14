#ifndef LINAUTHENTICATIONDIALOG_H
#define LINAUTHENTICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class LinAuthenticationDialog;
}

class LinAuthenticationDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinAuthenticationDialog(QWidget *parent = 0);
  ~LinAuthenticationDialog();

  QString getEmail();
  QString getPasswd();

  void hideLabel();
  void showLabel();
  
private slots:
  void on_pwChecBox_toggled(bool checked);

private:
  Ui::LinAuthenticationDialog *ui;
};

#endif // LINAUTHENTICATIONDIALOG_H
