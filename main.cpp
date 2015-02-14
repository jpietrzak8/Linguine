//
// main.cpp
//
// Copyright 2014 by John Pietrzak (jpietrzak8@gmail.com)
//
// This file is part of Linguine.
//
// Linguine is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Linguine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Linguine; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QWidget>
#include "qmaemo5homescreenadaptor.h"
#include "linwidgetuiform.h"
#include <QDebug>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Are we running as a widget?
  bool runAsApp = false;
  QStringList argList = app.arguments();
  int i = 1;
  while (i < argList.size())
  {
    if ( (argList.at(i) == "-a")
      || (argList.at(i) == "--runAsApp"))
    {
      runAsApp = true;
      break;
    }

    ++i;
  }

  if (!runAsApp)
  {
    // We are running as a widget:
    LinWidgetUIForm widget;

    QMaemo5HomescreenAdaptor *adaptor =
      new QMaemo5HomescreenAdaptor(&widget);

    adaptor->setSettingsAvailable(true);

    QObject::connect(
      adaptor,
      SIGNAL(settingsRequested()),
      &widget,
      SLOT(showSettingsDialog()));

    widget.show();

    return app.exec();
  }

  // Otherwise, we are running as a normal app:

  app.setApplicationName("Linguine");

  MainWindow mainWindow;
  mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
  mainWindow.showExpanded();

  return app.exec();
}
