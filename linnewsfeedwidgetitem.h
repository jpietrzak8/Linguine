#ifndef LINNEWSFEEDWIDGETITEM_H
#define LINNEWSFEEDWIDGETITEM_H

#include <QListWidgetItem>
#include <QPixmap>
#include <QString>

class QSettings;
class LinRSSParser;
class QNetworkAccessManager;

enum Frequency
{
  Hourly_Rate,
  Daily_Rate,
  Weekly_Rate,
  Other_Rate
};


enum MediaType
{
  Audio_Media,
  Video_Media
};


enum ContentType
{
  News_Content,
  Politics_Content,
  Other_Content
};


class LinNewsfeedWidgetItem: public QListWidgetItem
{
public:
  LinNewsfeedWidgetItem(
    QString name,
    QString sourceUrl,
    Frequency freq,
    ContentType category,
    MediaType media,
    QNetworkAccessManager *qnam);

  LinNewsfeedWidgetItem(
    QString name,
    QString sourceUrl,
    Frequency freq,
    ContentType category,
    MediaType media,
    QSettings &settings,
    QNetworkAccessManager *qnam);

  void parseRSS();

  void setImage(
    const QByteArray &ba);

  void setItemTitle(
    QString itemTitle);

  void setMediaUrl(
    QString mediaUrl);

  QPixmap getImage();
  QString getItemTitle();
  QString getMediaUrl();

  void setFrequency(
    Frequency f)
    {frequency = f;}

  void setMediaType(
    MediaType m)
    {media = m;}

  void setContentType(
    ContentType c)
    {category = c;}

  Frequency getFrequency() {return frequency;}
  MediaType getMediaType() {return media;}
  ContentType getContentType() {return category;}

private:
  QString name;
  QString sourceUrl;
  Frequency frequency;
  ContentType category;
  MediaType media;

  LinRSSParser *parser;
  QPixmap image;
  QString itemTitle;
  QString mediaUrl;
};

#endif // LINNEWSFEEDWIDGETITEM_H
