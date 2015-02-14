//
// linwidgetsettingsdialog.h
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
#ifndef LINWIDGETSETTINGSDIALOG_H
#define LINWIDGETSETTINGSDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include "linnewsfeedwidgetitem.h"
#include "linmaemo5theme.h"

class QXmlStreamReader;
class QString;
class QSettings;

namespace Ui {
class LinWidgetSettingsDialog;
}

class LinWidgetSettingsDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinWidgetSettingsDialog(
    QWidget *parent = 0);

  ~LinWidgetSettingsDialog();

  LinNewsfeedWidgetItem *getCurrentItem();

  QString getBackgroundColor()
    { return themeSettings.getBackgroundColor(); }

  QString getDefaultTextColor()
    { return themeSettings.getDefaultTextColor(); }

  QString getActiveTextColor()
    { return themeSettings.getActiveTextColor(); }

  QString getSecondaryTextColor()
    { return themeSettings.getSecondaryTextColor(); }

  QString getAccentColor()
    { return themeSettings.getAccentColor(); }

  QString getSystemFontFamily()
    { return themeSettings.getSystemFontFamily(); }

signals:
  void newsfeedSelected(
    LinNewsfeedWidgetItem *item);
  
private slots:
  void on_newsfeedListWidget_itemActivated(
    QListWidgetItem *item);

private:
  void retrieveNewsfeeds(
    QSettings &settings);

  void parseXML(
    QXmlStreamReader &reader);

  void parseLinguineElement(
    QXmlStreamReader &reader);

  void parseNewsfeedElement(
    QString &name,
    TagCollection &tags,
    QXmlStreamReader &reader);

  QString parseText(
    QXmlStreamReader &reader,
    QString elementName);

  FrequencyType parseFrequency(
    QString freqStr);

  MediaType parseMedia(
    QString mediaStr);

  LinFormatType parseFormat(
    QString formatStr);

  Ui::LinWidgetSettingsDialog *ui;

  QNetworkAccessManager qnam;
  LinMaemo5Theme themeSettings;
};

#endif // LINWIDGETSETTINGSDIALOG_H
