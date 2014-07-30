#ifndef LINCATEGORYDIALOG_H
#define LINCATEGORYDIALOG_H

#include <QDialog>

namespace Ui {
class LinCategoryDialog;
}

class LinCategoryDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinCategoryDialog(QWidget *parent = 0);
  ~LinCategoryDialog();
  
private:
  Ui::LinCategoryDialog *ui;
};

#endif // LINCATEGORYDIALOG_H
