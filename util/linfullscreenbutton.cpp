// Based on Qt/Nokia example "fullscreenexitbutton.h"

#include "linfullscreenbutton.h"

#include <QEvent>

#include "LinVideoDisplayForm.h"
//#include "LinPhononDisplayForm.h"

LinFullScreenButton::LinFullScreenButton(
  LinVideoDisplayForm *vdf)
  : QToolButton(vdf)
{
  setIcon(QIcon::fromTheme("general_fullsize"));

  setFixedSize(sizeHint());

  QPalette pal = palette();
  QColor backgroundColor = pal.color(backgroundRole());
  backgroundColor.setAlpha(128);  // apparently, this is 0.5 transparency
  pal.setColor(backgroundRole(), backgroundColor);
  setPalette(pal);

  setAutoFillBackground(true);

  setVisible(false);

  connect(this, SIGNAL(clicked()), vdf, SLOT(returnFromFullscreen()));

  vdf->installEventFilter(this); // is this be best way to do it?
}


/*
LinFullScreenButton::LinFullScreenButton(
  LinPhononDisplayForm *pdf)
  : QToolButton(pdf)
{
  setIcon(QIcon::fromTheme("general_fullsize"));

  setFixedSize(sizeHint());

  QPalette pal = palette();
  QColor backgroundColor = pal.color(backgroundRole());
  backgroundColor.setAlpha(128);  // apparently, this is 0.5 transparency
  pal.setColor(backgroundRole(), backgroundColor);
  setPalette(pal);

  setAutoFillBackground(true);

  setVisible(false);

  connect(this, SIGNAL(clicked()), pdf, SLOT(returnFromFullscreen()));

  pdf->installEventFilter(this); // is this be best way to do it?
}
*/


bool LinFullScreenButton::eventFilter(
  QObject *object,
  QEvent *event)
{
  if (object != parent())
  {
    return QToolButton::eventFilter(object, event);
  }

  QWidget *parent = parentWidget();
  bool isFullScreen = parent->windowState() & Qt::WindowFullScreen;

  if (event->type() == QEvent::WindowStateChange)
  {
    setVisible(isFullScreen);
    if (isFullScreen) raise();
    relocateIcon(parent);
  }
  else if (event->type() == QEvent::Resize)
  {
    relocateIcon(parent);
  }

  return QToolButton::eventFilter(object, event);
}


void LinFullScreenButton::relocateIcon(
  QWidget *parent)
{
  if (isVisible())
  {
    move(
      parent->width() - width(),
      parent->height() - height());
  }
}
