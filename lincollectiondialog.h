#ifndef LINCOLLECTIONDIALOG_H
#define LINCOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class LinCollectionDialog;
}

class LinCollectionDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinCollectionDialog(QWidget *parent = 0);
  ~LinCollectionDialog();
  
private:
  Ui::LinCollectionDialog *ui;
};

#endif // LINCOLLECTIONDIALOG_H
