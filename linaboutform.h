#ifndef LINABOUTFORM_H
#define LINABOUTFORM_H

#include <QWidget>

namespace Ui {
class LinAboutForm;
}

class LinAboutForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinAboutForm(QWidget *parent = 0);
  ~LinAboutForm();
  
private:
  Ui::LinAboutForm *ui;
};

#endif // LINABOUTFORM_H
