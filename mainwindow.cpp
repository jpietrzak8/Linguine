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

#include "linflickabletabbar.h"
#include "linnowplayingform.h"
#include "linvideodisplayform.h"
//#include "linhtmldisplayform.h"
#include "linnativedisplayform.h"
#include "lintordisplayform.h"
#include "linpreferencesform.h"
#include "lindocumentationform.h"
#include "linaboutform.h"
#include "linnewsfeedwidgetitem.h"
#include "lincollectionwidgetitem.h"
#include "linauthenticationdialog.h"
//#include "linfilterdialog.h"
//#include "lindbus.h"
#include "linfeedsource.h"
//#include "lintormanager.h"
//#include "lintoritems.h"
#include "linauthenticationdialog.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QString>
#include <QSettings>
#include <QMaemo5InformationBox>
#include <QFileDialog>
#include <QAbstractKineticScroller>

#include <QDebug>

MainWindow::MainWindow(
  QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    nowPlayingForm(0),
    videoDisplayForm(0),
    nativeDisplayForm(0),
    torDisplayForm(0),
    torAuthenticationDialog(0),
    preferencesForm(0),
    documentationForm(0),
    aboutForm(0),
    dbus(0),
    nativeAlreadySetup(false),
    nativeFlickableTabBar(0),
    torAlreadySetup(false),
    torFlickableTabBar(0)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);

//  nowPlayingForm = new LinNowPlayingForm(this);
//  videoDisplayForm = new LinVideoDisplayForm(this);
  nativeDisplayForm = new LinNativeDisplayForm(this, &qnam);
  preferencesForm = new LinPreferencesForm(this);
  documentationForm = new LinDocumentationForm(this);
  aboutForm = new LinAboutForm(this);
  dbus = new LinDBus();

  setupFeedSources();

  ui->mainStackedWidget->setCurrentWidget(ui->feedSelectorPage);

  QSettings settings("pietrzak.org", "Linguine");

  if (settings.contains("visibleWidget"))
  {
    QString widgetName = settings.value("visibleWidget").toString();
    if (widgetName == "NativeFeed")
    {
      setupNativeUI();
      ui->mainStackedWidget->setCurrentWidget(ui->nativeFeedPage);
    }
    else if (widgetName == "TORFeed")
    {
      setupTORUI();

      torAuthenticationDialog->hideLabel();

      if (torAuthenticationDialog->exec() == QDialog::Rejected)
      {
        // Fall back to source selector:
        ui->mainStackedWidget->setCurrentWidget(ui->feedSelectorPage);
      }
      else
      {
        torManager->authenticate(
          torAuthenticationDialog->getEmail(),
          torAuthenticationDialog->getPasswd());

        ui->mainStackedWidget->setCurrentWidget(ui->torFeedPage);
      }
    }
  }
}


MainWindow::~MainWindow()
{
  QSettings settings("pietrzak.org", "Linguine");

  if (ui->mainStackedWidget->currentWidget() == ui->feedSelectorPage)
  {
    settings.setValue("visibleWidget", "FeedSelector");
  }
  else if (ui->mainStackedWidget->currentWidget() == ui->nativeFeedPage)
  {
    settings.setValue("visibleWidget", "NativeFeed");
  }
  else if (ui->mainStackedWidget->currentWidget() == ui->torFeedPage)
  {
    settings.setValue("visibleWidget", "TORFeed");
  }

  if (nowPlayingForm) delete nowPlayingForm;
  if (videoDisplayForm) delete videoDisplayForm;
  if (nativeDisplayForm) delete nativeDisplayForm;
  if (torDisplayForm) delete torDisplayForm;
  if (preferencesForm) delete preferencesForm;
  if (documentationForm) delete documentationForm;
  if (aboutForm) delete aboutForm;
  if (dbus) delete dbus;
  if (nativeFlickableTabBar) delete nativeFlickableTabBar;
  if (torFlickableTabBar) delete torFlickableTabBar;

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


void MainWindow::retrieveNewsfeeds(
  QSettings &settings)
{
  // Read the categories out of the settings:
  int size = settings.beginReadArray("collections");

  if (size == 0)
  {
    // Try to read the categories out of the newsfeeds file:
    QFile xmlFile(":/newsfeeds.xml");

    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString err("Unable to open default XML file");

      QMaemo5InformationBox::information(this, err);

      qWarning() << err;

      return;
    }

    QXmlStreamReader reader(&xmlFile);

    parseXML(reader);

    return;
  }

  // Otherwise, continue onward:
  int index = 0;
  int tagsSize = 0;
  int tagsIndex = 0;
  QString name;
  QString url;
  FrequencyType freq;
  MediaType media;
  LanguageType language;
  QSet<QString> tags;

  while (index < size)
  {
    settings.setArrayIndex(index);

    name = settings.value("name").toString();
    freq = (FrequencyType) settings.value("frequency").toInt();
    media = (MediaType) settings.value("media").toInt();
    language = (LanguageType) settings.value("language").toInt();

    tagsSize = settings.beginReadArray("requiredTags");
    if (tagsSize)
    {
      tagsIndex = 0;
      while (tagsIndex < tagsSize)
      {
        tags.insert(settings.value("requiredTag").toString());
        ++tagsIndex;
      }
    }
    settings.endArray();

    LinCollectionWidgetItem *cwi =
      new LinCollectionWidgetItem(name, freq, media, language, tags);

    tags.clear();

    nativeFlickableTabBar->addItem(cwi);

    ++index;
  }

  settings.endArray();

  nativeFlickableTabBar->setCurrentRow(0);

  size = settings.beginReadArray("newsfeeds");

  // Read the newsfeeds out of the settings:
  index = 0;
  while (index < size)
  {
    settings.setArrayIndex(index);

    name = settings.value("name").toString();
    url = settings.value("url").toString();
    freq = (FrequencyType) settings.value("frequency").toInt();
    media = (MediaType) settings.value("media").toInt();
    language = (LanguageType) settings.value("language").toInt();

    tagsSize = settings.beginReadArray("tags");
    if (tagsSize)
    {
      tagsIndex = 0;
      while (tagsIndex < tagsSize)
      {
        tags.insert(settings.value("tag").toString());
        ++tagsIndex;
      }
    }
    settings.endArray();

    LinNewsfeedWidgetItem *nwi =
      new LinNewsfeedWidgetItem(
        name,
        url,
        freq,
        media,
        language,
        tags,
        getActiveTextColor(),
        &qnam);

    tags.clear();

    ui->nativeMediaList->addItem(nwi);

    // Safe to do this here, because all collections already defined:
    filterItem(nwi);

    ++index;
  }

  settings.endArray();
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

    qWarning() << err;
  }
}


void MainWindow::parseLinguineElement(
  QXmlStreamReader &reader)
{
  // Remove all collections from the tab bar:
  nativeFlickableTabBar->clear();
  // Remove all newsfeeds from the current list:
  ui->nativeMediaList->clear();

  QString name;
  QString url;
  QString refreshString;
  QString catString;
  QString mediaString;
  FrequencyType freq = Any_Rate;
  MediaType media = Any_Media;
  LanguageType language = Any_Language;
  QSet<QString> tags;

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
          freq = parseFrequency(
            reader.attributes().value("frequency").toString());
        }

        if (reader.attributes().hasAttribute("media"))
        {
          media = parseMedia(
            reader.attributes().value("media").toString());
        }

        parseNewsfeedElement(name, tags, reader);

        LinNewsfeedWidgetItem *nwi =
          new LinNewsfeedWidgetItem(
            name,
            url,
            freq,
            media,
            language,
            tags,
            getActiveTextColor(),
            &qnam);

        ui->nativeMediaList->addItem(nwi);

        nwi->setHidden(true);

        // Reset all the fields:
        name.clear();
        url.clear();
        freq = Any_Rate;
        media = Any_Media;
        language = Any_Language;
        tags.clear();
      }
      else if (reader.name() == "collection")
      {
        if (reader.attributes().hasAttribute("frequency"))
        {
          freq = parseFrequency(
            reader.attributes().value("frequency").toString());
        }

        if (reader.attributes().hasAttribute("media"))
        {
          media = parseMedia(
            reader.attributes().value("media").toString());
        }

        parseCollectionElement(name, tags, reader);

        LinCollectionWidgetItem *cwi =
          new LinCollectionWidgetItem(
            name,
            freq,
            media,
            language,
            tags);

        nativeFlickableTabBar->addItem(cwi);

        // Reset all the fields:
        name.clear();
        freq = Any_Rate;
        media = Any_Media;
        language = Any_Language;
        tags.clear();
      }
    }
  }

  // Update the settings:
  QSettings settings("pietrzak.org", "Linguine");

  // First, remove all existing categories and newsfeeds from settings:
  settings.remove("categories");
  settings.remove("newsfeeds");

  // Start a fresh array of categories:
  settings.beginWriteArray("categories");

  int index = 0;
  int count = nativeFlickableTabBar->count();
  while (index < count)
  {
    LinCollectionWidgetItem *cwi =
      dynamic_cast<LinCollectionWidgetItem *>(
        nativeFlickableTabBar->item(index));

    settings.setArrayIndex(index);
    cwi->addToSettings(settings);

    ++index;
  }

  settings.endArray();

  // Start a fresh array of newsfeeds:
  settings.beginWriteArray("newsfeeds");

  index = 0;
  count = ui->nativeMediaList->count();
  while (index < count)
  {
    LinNewsfeedWidgetItem *nwi =
      dynamic_cast<LinNewsfeedWidgetItem *>(ui->nativeMediaList->item(index));

    settings.setArrayIndex(index);
    nwi->addToSettings(settings);

    ++index;
  }

  settings.endArray();

  // If at this point we have no collections, create one:
  if (nativeFlickableTabBar->count() == 0)
  {
    tags.clear(); // This should be unnecessary

    LinCollectionWidgetItem *everything =
      new LinCollectionWidgetItem(
        "Everything",
        Any_Rate,
        Any_Media,
        Any_Language,
        tags);

    nativeFlickableTabBar->addItem(everything);
  }

  nativeFlickableTabBar->setCurrentRow(0);

  refilter(
    nativeFlickableTabBar->currentItem());
}


void MainWindow::parseNewsfeedElement(
  QString &name,
  QSet<QString> &tags,
  QXmlStreamReader &reader)
{
  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isStartElement())
    {
      if (reader.name() == "name")
      {
        name = parseText(reader, "name");
      }
      else if (reader.name() == "tag")
      {
        tags.insert(parseText(reader, "tag"));
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
}


void MainWindow::parseCollectionElement(
  QString &name,
  QSet<QString> &tags,
  QXmlStreamReader &reader)
{
  while (!reader.atEnd())
  {
    reader.readNext();

    if (reader.isStartElement())
    {
      if (reader.name() == "name")
      {
        name = parseText(reader, "name");
      }
      else if (reader.name() == "requiredTag")
      {
        tags.insert(parseText(reader, "requiredTag"));
      }
    }
    else if (reader.isEndElement())
    {
      if (reader.name() == "collection")
      {
        break;
      }
    }
  }
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
  if (nativeFlickableTabBar->matchesCurrentCollection(nwi))
  {
    nwi->setHidden(false);
    nwi->parseRSS();
  }
  else
  {
    nwi->setHidden(true);
  }
}


void MainWindow::on_sourcesListWidget_itemActivated(QListWidgetItem *item)
{
  LinFeedSource *fs = dynamic_cast<LinFeedSource *>(item);

  switch (fs->getType())
  {
  case TheOldReader_Source:
    setupTORUI();

    torAuthenticationDialog->hideLabel();

    if (torAuthenticationDialog->exec() == QDialog::Rejected)
    {
      // User canceled logging in to TOR, so get out:
      break;
    }

    torManager->authenticate(
      torAuthenticationDialog->getEmail(),
      torAuthenticationDialog->getPasswd());

    ui->mainStackedWidget->setCurrentWidget(ui->torFeedPage);
    break;

//  case Facebook_Source:
//    break;

  case Native_Source:
  default:
    if (!nativeAlreadySetup)
    {
      setupNativeUI();
    }

    ui->mainStackedWidget->setCurrentWidget(ui->nativeFeedPage);
    break;
  }
}
  

void MainWindow::on_nativeMediaList_itemActivated(QListWidgetItem *item)
{
  if (!item) return;

  LinNewsfeedWidgetItem *nwi = dynamic_cast<LinNewsfeedWidgetItem *>(item);

  if (!nwi) return;

  MediaType media = nwi->getMedia();

  if (media == Audio_Media)
  {
    if (preferencesForm->openExternalPlayer())
    {
      dbus->launchMedia(nwi->getMediaUrl());
    }
    else
    {
      // Make sure the video form isn't running:
//      videoDisplayForm->stopPlaying();

      if (!nowPlayingForm)
      {
        nowPlayingForm = new LinNowPlayingForm(this);
      }

      nowPlayingForm->setProgram(
        nwi->getName(),
        nwi->getImage(),
        nwi->getItemTitle(),
        nwi->getItemSummary(),
        nwi->getItemPubDate(),
        nwi->getMediaUrl());

      nowPlayingForm->show();
    }
  }
  else if (media == Video_Media)
  {
    if (preferencesForm->openExternalPlayer())
    {
      dbus->launchMedia(nwi->getMediaUrl());
    }
    else
    {
      // Make sure the audio form isn't playing:
//      nowPlayingForm->stopPlaying();
      if (!videoDisplayForm)
      {
        videoDisplayForm = new LinVideoDisplayForm(this);
      }

      videoDisplayForm->setProgram(
        nwi->getName(),
        nwi->getItemSummary(),
        nwi->getItemPubDate(),
        nwi->getMediaUrl());

      // Set auto orientation to false:
      setAttribute(static_cast<Qt::WidgetAttribute>(130), false);

      // Set landscape orientation to true:
      setAttribute(static_cast<Qt::WidgetAttribute>(129), true);

      videoDisplayForm->show();
    }
  }
  else if (media == Text_Media)
  {
    nativeDisplayForm->displayText(
      nwi->getName(),
      nwi->getSourceUrl(),
      nwi->getFaviconUrl(),
      preferencesForm->hideImages(),
      preferencesForm->openExternalBrowser());

    nativeDisplayForm->show();
  }
/*
  else if (media == Image_Media)
  {
    imageDisplayForm->displayImage(
      nwi->getSomethingUrl());

    imageDisplayForm->show();
  }
*/
}


void MainWindow::on_torTreeWidget_itemActivated(
  QTreeWidgetItem *item, int column)
{
  torDisplayForm->setupTORDisplay(
    item->text(0),
    preferencesForm->openExternalPlayer(),
    preferencesForm->openExternalBrowser());

  torManager->retrieveTORItems(item);

  torDisplayForm->show();
}


void MainWindow::on_actionSelect_New_Source_triggered()
{
  ui->mainStackedWidget->setCurrentWidget(ui->feedSelectorPage);
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

    qWarning() << err;

    return;
  }

  QXmlStreamReader reader(&xmlFile);

  parseXML(reader);
}


void MainWindow::on_actionReset_Newsfeeds_triggered()
{
  QFile xmlFile(":/newsfeeds.xml");

  if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString err("Unable to open default XML file");

    QMaemo5InformationBox::information(this, err);

    qWarning() << err;

    return;
  }

  QXmlStreamReader reader(&xmlFile);

  parseXML(reader);
}


void MainWindow::on_actionPreferences_triggered()
{
  preferencesForm->show();
}


void MainWindow::on_actionDocumentation_triggered()
{
  documentationForm->show();
}


void MainWindow::on_actionAbout_triggered()
{
  aboutForm->show();
}


void MainWindow::refilter(
  QListWidgetItem *tabItem)
{
/*
  QAbstractKineticScroller *scroller =
    flickableTabBar->property("kineticScroller")
      .value<QAbstractKineticScroller *>();

  if (scroller->state() != QAbstractKineticScroller::Inactive)
  {
    // Don't do anything if the user is pushing the tab bar around...
    return;
  }
*/

  int count = ui->nativeMediaList->count();

  int index = 0;

  while (index < count)
  {
    LinNewsfeedWidgetItem *nwi = 
      dynamic_cast<LinNewsfeedWidgetItem *>(ui->nativeMediaList->item(index));

    filterItem(nwi);

    ++index;
  }
}


FrequencyType MainWindow::parseFrequency(
  QString freqStr)
{
  if (freqStr == "hourly")
  {
    return Hourly_Rate;
  }
  else if (freqStr == "daily")
  {
    return Daily_Rate;
  }
  else if (freqStr == "weekly")
  {
    return Weekly_Rate;
  }
  else if (freqStr == "other")
  {
    return Other_Rate;
  }
  else if (freqStr == "any")
  {
    return Any_Rate;
  }

  qWarning() << "Could not parse frequency: " << freqStr;

  // Bit of a hack:
  return Other_Rate;
}


MediaType MainWindow::parseMedia(
  QString mediaStr)
{
  if (mediaStr == "audio")
  {
    return Audio_Media;
  }
  else if (mediaStr == "video")
  {
    return Video_Media;
  }
  else if (mediaStr == "text")
  {
    return Text_Media;
  }
//  else if (mediaStr == "image")
//  {
//    return Image_Media;
//  }
  else if (mediaStr == "any")
  {
    return Any_Media;
  }

  qWarning() << "Could not parse media: " << mediaStr;

  // This is definitely a hack:
  return Text_Media;
}


void MainWindow::setupFeedSources()
{
  ui->sourcesListWidget->addItem(
    new LinNativeFeedSource());

  ui->sourcesListWidget->addItem(
    new LinTORFeedSource());

//  ui->sourcesListWidget->addItem(
//    new LinFacebookFeedSource());
}


void MainWindow::setupNativeUI()
{
  nativeFlickableTabBar = new LinFlickableTabBar(this);
  ui->nativeLayout->insertWidget(0, nativeFlickableTabBar);

  connect(
    nativeFlickableTabBar,
    SIGNAL(itemActivated(QListWidgetItem *)),
    this,
    SLOT(refilter(QListWidgetItem *)));

  QSettings settings("pietrzak.org", "Linguine");

  retrieveNewsfeeds(settings);

  nativeAlreadySetup = true;
}


void MainWindow::setupTORUI()
{
  if (torAlreadySetup)
  {
    return;
  }

  torManager = new LinTORManager(
    &qnam,
    ui->torTreeWidget);

  torDisplayForm = new LinTORDisplayForm(this, &qnam);

  torAuthenticationDialog = new LinAuthenticationDialog();

  connect(
    torManager,
    SIGNAL(loginFailed()),
    this,
    SLOT(retryTORLogin()));

  connect(
    torManager,
    SIGNAL(itemsReady()),
    this,
    SLOT(displayTORItems()));

  torAlreadySetup = true;
}


void MainWindow::displayTORItems()
{
  torDisplayForm->displayItems(
    torManager->getTORItemCollection());
}


void MainWindow::retryTORLogin()
{
  torAuthenticationDialog->showLabel();

  if (torAuthenticationDialog->exec() == QDialog::Rejected)
  {
    // User canceled logging in to TOR, so get out:
    ui->mainStackedWidget->setCurrentWidget(ui->feedSelectorPage);
    return;
  }

  torManager->authenticate(
    torAuthenticationDialog->getEmail(),
    torAuthenticationDialog->getPasswd());
}
