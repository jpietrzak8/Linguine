//
// linwidgetuiform.h
//
// Copyright 2015 by John Pietrzak (jpietrzak8@gmail.com)
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

#ifndef LINWIDGETUIFORM_H
#define LINWIDGETUIFORM_H

#include <QWidget>

class LinDBus;
class LinWidgetSettingsDialog;
class LinNewsfeedWidgetItem;
//class QString;
class LinRSSParser;
class LinAtomParser;
class QTimer;

namespace Ui {
class LinWidgetUIForm;
}

class LinWidgetUIForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LinWidgetUIForm(QWidget *parent = 0);
  ~LinWidgetUIForm();

public slots:
  void showSettingsDialog();

private slots:
  void on_newsfeedButton_clicked();

  void newsfeedChanged(
    LinNewsfeedWidgetItem *item);

  void updateItem();

  void updateDisplay();

private:
  Ui::LinWidgetUIForm *ui;

  LinDBus *dbus;
  LinWidgetSettingsDialog *settingsDialog;
//  QString mediaUrl;
  LinNewsfeedWidgetItem *currentItem; // Not owned here!

  LinRSSParser *rssParser;
  LinAtomParser *atomParser;
  QTimer *updateTimer;
};

#endif // LINWIDGETUIFORM_H
