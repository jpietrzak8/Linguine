//
// linwidgetsettingsdialog.cpp
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

#include "linwidgetsettingsdialog.h"
#include "ui_linwidgetsettingsdialog.h"

#include <QSettings>
#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include <QMaemo5InformationBox>
#include <QDebug>

LinWidgetSettingsDialog::LinWidgetSettingsDialog(
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::LinWidgetSettingsDialog)
{
  ui->setupUi(this);

  QSettings settings("pietrzak.org", "Linguine");

  retrieveNewsfeeds(settings);

  if (settings.contains("WidgetSelectedRow"))
  {
    ui->newsfeedListWidget->setCurrentRow(
      settings.value("WidgetSelectedRow").toInt());
  }
}


LinWidgetSettingsDialog::~LinWidgetSettingsDialog()
{

  QSettings settings("pietrzak.org", "Linguine");

  settings.setValue("WidgetSelectedRow", ui->newsfeedListWidget->currentRow());

  delete ui;
}


LinNewsfeedWidgetItem *LinWidgetSettingsDialog::getCurrentItem()
{
  QListWidgetItem *qlwi = ui->newsfeedListWidget->currentItem();

  if (!qlwi)
  {
    qlwi = ui->newsfeedListWidget->item(0);
  }

  return dynamic_cast<LinNewsfeedWidgetItem *>(qlwi);
}


void LinWidgetSettingsDialog::retrieveNewsfeeds(
  QSettings &settings)
{
  int size = settings.beginReadArray("newsfeeds");

  if (size == 0)
  {
    // Try to read the newsfeeds out of the newsfeeds file:
    QFile xmlFile(":/newsfeeds.xml");

    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QString err("Unable to open default XML file");

      QMaemo5InformationBox::information(0, err);

      qWarning() << err;

      return;
    }

    QXmlStreamReader reader(&xmlFile);

    parseXML(reader);

    return;
  }

  // Read the newsfeeds out of the settings:
  int index = 0;
  int tagsSize = 0;
  int tagsIndex = 0;
  QString name;
  QString url;
  FrequencyType freq;
  MediaType media;
  LanguageType language;
  LinFormatType format;
  TagCollection tags;

  while (index < size)
  {
    settings.setArrayIndex(index);

    name = settings.value("name").toString();
    url = settings.value("url").toString();
    freq = (FrequencyType) settings.value("frequency").toInt();
    media = (MediaType) settings.value("media").toInt();
    language = (LanguageType) settings.value("language").toInt();
    format = (LinFormatType) settings.value("format").toInt();

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
        format,
        tags,
        getActiveTextColor(),
        &qnam);

    ui->newsfeedListWidget->addItem(nwi);

    if (nwi->getMedia() == Audio_Media)
    {
      nwi->setHidden(false);
//      nwi->parseFeed();
    }
    else
    {
      nwi->setHidden(true);
    }

    tags.clear();

    ++index;
  }

  settings.endArray();
}


void LinWidgetSettingsDialog::on_newsfeedListWidget_itemActivated(
  QListWidgetItem *item)
{
  if (!item) return;

  LinNewsfeedWidgetItem *nwi = dynamic_cast<LinNewsfeedWidgetItem *>(item);

  if (!nwi) return;

  emit newsfeedSelected(nwi);
  accept();
}


void LinWidgetSettingsDialog::parseXML(
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

    QMaemo5InformationBox::information(0, err);

    qWarning() << err;
  }
}


void LinWidgetSettingsDialog::parseLinguineElement(
  QXmlStreamReader &reader)
{
  QString name;
  QString url;
  FrequencyType freq = Any_Rate;
  MediaType media = Any_Media;
  LanguageType language = Any_Language;
  LinFormatType format = RSS_Format;
  TagCollection tags;

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

        if (reader.attributes().hasAttribute("format"))
        {
          format = parseFormat(
            reader.attributes().value("format").toString());
        }

        parseNewsfeedElement(name, tags, reader);

        LinNewsfeedWidgetItem *nwi =
          new LinNewsfeedWidgetItem(
            name,
            url,
            freq,
            media,
            language,
            format,
            tags,
            getActiveTextColor(),
            &qnam);

        ui->newsfeedListWidget->addItem(nwi);

        if ( (nwi->getMedia() == Audio_Media) 
          || (nwi->getMedia() == Video_Media))
        {
          nwi->setHidden(false);
//          nwi->parseFeed(); // do this at last minute
        }
        else
        {
          nwi->setHidden(true);
        }

        // Reset all the fields:
        name.clear();
        url.clear();
        freq = Any_Rate;
        media = Any_Media;
        language = Any_Language;
        tags.clear();
      }
    }
  }
}


void LinWidgetSettingsDialog::parseNewsfeedElement(
  QString &name,
  TagCollection &tags,
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


QString LinWidgetSettingsDialog::parseText(
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


FrequencyType LinWidgetSettingsDialog::parseFrequency(
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


MediaType LinWidgetSettingsDialog::parseMedia(
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


LinFormatType LinWidgetSettingsDialog::parseFormat(
  QString formatStr)
{
  if (formatStr == "atom")
  {
    return Atom_Format;
  }
  else if (formatStr == "rss")
  {
    return RSS_Format;
  }

  // For now, default to RSS:
  return RSS_Format;
}
