//
// mainwindow.cpp
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
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include "linnowplayingform.h"
#include "linvideodisplayform.h"
#include "lindocumentationform.h"
#include "linaboutform.h"
#include "linnewsfeedwidgetitem.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QString>
#include <QSettings>
#include <QMaemo5InformationBox>
#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(
  QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    nowPlayingForm(0),
    videoDisplayForm(0)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);

  nowPlayingForm = new LinNowPlayingForm(this);
  videoDisplayForm = new LinVideoDisplayForm(this);
  documentationForm = new LinDocumentationForm(this);
  aboutForm = new LinAboutForm(this);

  retrieveNewsfeeds();
}


MainWindow::~MainWindow()
{
  if (nowPlayingForm) delete nowPlayingForm;
  if (videoDisplayForm) delete videoDisplayForm;
  delete ui;
}


void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}


void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}


void MainWindow::retrieveNewsfeeds()
{
  QSettings settings("pietrzak.org", "Linguine");

  int size = settings.beginReadArray("newsfeeds");

//  if (size == 0)
  {
    QFile xmlFile(":/newsfeeds.xml");

    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString err("Unable to open default XML file");

      QMaemo5InformationBox::information(this, err);

      qDebug() << err;

      return;
    }

    QXmlStreamReader reader(&xmlFile);

    parseXML(reader);

    return;
  }

  // Otherwise, read the newsfeeds out of the settings:
  int index = 0;
  QString name;
  QString url;
  Frequency freq;
  ContentType category;
  MediaType media;
  while (index < size)
  {
    settings.setArrayIndex(index);

    name = settings.value("name").toString();
    url = settings.value("url").toString();
    freq = (Frequency) settings.value("frequency").toInt();
    category = (ContentType) settings.value("category").toInt();
    media = (MediaType) settings.value("media").toInt();

    LinNewsfeedWidgetItem *nwi =
      new LinNewsfeedWidgetItem(name, url, freq, category, media, &qnam);

    ui->mediaListWidget->addItem(nwi);

    filterItem(nwi);

    nwi->parseRSS();

    ++index;
  }
}


void MainWindow::parseXML(
  QXmlStreamReader &reader)
{
  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isStartElement())
    {
      if (reader.name() == "linguine")
      {
        parseLinguineElement(reader);
      }
    }
  }

  if (reader.hasError())
  {
    QString err;
    err.append("QXmlStreamReader returned error: ");
    err.append(reader.errorString());

    QMaemo5InformationBox::information(this, err);

    qDebug() << err;
  }
}


void MainWindow::parseLinguineElement(
  QXmlStreamReader &reader)
{
  // We'll update the settings as we go.
  QSettings settings("pietrzak.org", "Linguine");

  // First, remove all existing newsfeeds from settings:
  settings.remove("newsfeeds");

  // Also, remove all newsfeeds from the current list:
  ui->mediaListWidget->clear();

  // Now, start a fresh array of newsfeeds:
  settings.beginWriteArray("newsfeeds");

  int index = 0;

  QString name;
  QString url;
  QString refreshString;
  QString catString;
  QString mediaString;
  Frequency freq = Other_Rate;
  ContentType category = Other_Content;
  MediaType media = Audio_Media;

  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isStartElement())
    {
      if (reader.name() == "newsfeed")
      {
        if (reader.attributes().hasAttribute("url"))
        {
          url = reader.attributes().value("url").toString();
//          hasUrl= true;
        }

        if (reader.attributes().hasAttribute("frequency"))
        {
          // For now, default to "Other" rate:
//          freq = Other_Rate;

          refreshString = reader.attributes().value("frequency").toString();

          if (refreshString == "hourly")
          {
            freq = Hourly_Rate;
          }
          else if (refreshString == "daily")
          {
            freq = Daily_Rate;
          }
          else if (refreshString == "weekly")
          {
            freq = Weekly_Rate;
          }
        }

        if (reader.attributes().hasAttribute("category"))
        {
          catString = reader.attributes().value("category").toString();

          if (catString == "news")
          {
            category = News_Content;
          }
          else if (catString == "politics")
          {
            category = Politics_Content;
          }
        }

        if (reader.attributes().hasAttribute("media"))
        {
          // For now, default to audio media:
//          media = Audio_Media;

          mediaString = reader.attributes().value("media").toString();

          if (mediaString == "video")
          {
            media = Video_Media;
          }
        }

        name = parseNameElement(reader);

        settings.setArrayIndex(index++);
        LinNewsfeedWidgetItem *nwi =
          new LinNewsfeedWidgetItem(
            name, url, freq, category, media, settings, &qnam);

        ui->mediaListWidget->addItem(nwi);

        filterItem(nwi);

        nwi->parseRSS();

        // Reset all the fields:
        name.clear();
        url.clear();
        freq = Other_Rate;
        category = Other_Content;
        media = Audio_Media;
      }
    }
  }

  settings.endArray();
}


QString MainWindow::parseNameElement(
  QXmlStreamReader &reader)
{
  QString textString;

  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isStartElement())
    {
      if (reader.name() == "name")
      {
        textString = parseText(reader, "name");
      }
    }
    else if (reader.isEndElement())
    {
      if (reader.name() == "newsfeed")
      {
        break;
      }
    }
  }

  return textString;
}


QString MainWindow::parseText(
  QXmlStreamReader &reader,
  QString elementName)
{
  QString textString;

  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isCharacters() && !reader.isWhitespace())
    {
      textString.append(reader.text().toString());
    }
    else if (reader.isEndElement())
    {
      if (reader.name() == elementName)
      {
        break;
      }
    }
  }

  return textString;
}


void MainWindow::filterItem(
  LinNewsfeedWidgetItem *nwi)
{
  // This is a hack:
  if ( nwi->getFrequency() !=
       (Frequency) ui->frequencyComboBox->currentIndex() )
  {
    nwi->setHidden(true);
    return;
  }

  // More hack:
  if (ui->categoryComboBox->currentIndex() != 2)
  {
    if ( nwi->getContentType() !=
         (ContentType) ui->categoryComboBox->currentIndex() )
    {
      nwi->setHidden(true);
      return;
    }
  }

  nwi->setHidden(false);
}
  

void MainWindow::on_mediaListWidget_itemActivated(QListWidgetItem *item)
{
  if (!item) return;

  LinNewsfeedWidgetItem *nwi = dynamic_cast<LinNewsfeedWidgetItem *>(item);

  if (!nwi) return;

  MediaType media = nwi->getMediaType();

  if (media == Audio_Media)
  {
    // Make sure the other form isn't playing:
    videoDisplayForm->stopPlaying();

    nowPlayingForm->setProgram(
      nwi->getImage(),
      nwi->getItemTitle(),
      nwi->getMediaUrl());

    nowPlayingForm->show();
  }
  else if (media == Video_Media)
  {
    // Make sure the other form isn't playing:
    nowPlayingForm->stopPlaying();

    videoDisplayForm->setProgram(
      nwi->getItemTitle(),
      nwi->getMediaUrl());

    videoDisplayForm->show();
  }
}


void MainWindow::on_actionLoad_Newsfeeds_File_triggered()
{
  QString filename = QFileDialog::getOpenFileName(
    this, "Select Webcam XML File");

  if (filename.isEmpty())
  {
    // The user did not choose a file.
    return;
  }

  QFile xmlFile(filename);

  if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString err;
    err += "Unable to open ";
    err += filename;

    QMaemo5InformationBox::information(this, err);

    qDebug() << err;

    return;
  }

  QXmlStreamReader reader(&xmlFile);

  parseXML(reader);
}


void MainWindow::on_actionDocumentation_triggered()
{
  documentationForm->show();
}


void MainWindow::on_actionAbout_triggered()
{
  aboutForm->show();
}


void MainWindow::refilter()
{
  int count = ui->mediaListWidget->count();

  int index = 0;

  while (index < count)
  {
    LinNewsfeedWidgetItem *nwi = 
      dynamic_cast<LinNewsfeedWidgetItem *>(ui->mediaListWidget->item(index));

    filterItem(nwi);

    ++index;
  }
}


void MainWindow::on_frequencyComboBox_currentIndexChanged(int index)
{
  refilter();
}


void MainWindow::on_categoryComboBox_currentIndexChanged(int index)
{
  refilter();
}
