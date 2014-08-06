#ifndef LINIMAGEDISPLAYFORM_H
#define LINIMAGEDISPLAYFORM_H

#include <QWidget>

#include <QPixmap>

namespace Ui {
class LinImageDisplayForm;
}

class LinImageDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinImageDisplayForm(QWidget *parent = 0);
  ~LinImageDisplayForm();

  void displayImage(
    something);
  
private slots:
  void on_actionZoom_In_triggered();

  void on_actionZoom_Out_triggered();

  void on_actionNative_Resolution_triggered();

  void on_actionFit_to_Window_triggered();

private:
  Ui::LinImageDisplayForm *ui;

  QPixmap currentPixmap;
};

#endif // LINIMAGEDISPLAYFORM_H
