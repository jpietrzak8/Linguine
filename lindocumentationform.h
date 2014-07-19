#ifndef LINDOCUMENTATIONFORM_H
#define LINDOCUMENTATIONFORM_H

#include <QWidget>

namespace Ui {
class LinDocumentationForm;
}

class LinDocumentationForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinDocumentationForm(QWidget *parent = 0);
  ~LinDocumentationForm();
  
private:
  Ui::LinDocumentationForm *ui;
};

#endif // LINDOCUMENTATIONFORM_H
