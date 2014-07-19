#ifndef LINFULLSCREENBUTTON_H
#define LINFULLSCREENBUTTON_H

// Based on Qt/Nokia example "fullscreenexitbutton.h"

#include <QToolButton>
class QEvent;
class QWidget;
class LinVideoDisplayForm;

class LinFullScreenButton: public QToolButton
{
  Q_OBJECT

public:
  LinFullScreenButton(
    LinVideoDisplayForm *parent);

//  LinFullScreenButton(
//    LinPhononDisplayForm *parent);

protected:
  bool eventFilter(
    QObject *object,
    QEvent *event);

private:
  void relocateIcon(
    QWidget *parent);
};

#endif // LINFULLSCREENBUTTON_H
