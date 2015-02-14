#include "lintormanager.h"

#include <QNetworkAccessManager>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QTreeWidget>
#include <QHeaderView>
#include <QList>
#include <qjson/parser.h>
#include "linflickabletabbar.h"

#include <QDebug>

LinTORManager::LinTORManager(
  QNetworkAccessManager *network,
  QTreeWidget *qtw)
  : torTreeWidget(qtw),
    allSubsItem(0),
    qnam(network),
    authReply(0),
    foldersRequest(0),
    foldersReply(0),
    subscriptionsRequest(0),
    subscriptionsReply(0),
    itemIDsReply(0),
    torItemReply(0)
{
}


LinTORManager::~LinTORManager()
{
  if (foldersRequest) delete foldersRequest;
  if (subscriptionsRequest) delete subscriptionsRequest;
}


void LinTORManager::authenticate(
  QString email,
  QString passwd)
{
//  email = "jpietrzak8@gmail.com";
//  passwd = "Johns*TOR";
  // Do something to retrieve user/password strings...

  QString authUrl = "https://theoldreader.com/accounts/ClientLogin";

  QString authPost = "client=Linguine&accountType=HOSTED_OR_GOOGLE&service=reader&Email=";
  authPost += email;
  authPost += "&Passwd=";
  authPost += passwd;
  authPost += "&output=json";

  authReply =
    qnam->post(QNetworkRequest(QUrl(authUrl)), authPost.toUtf8());

  connect(
    authReply,
    SIGNAL(finished()),
    this,
    SLOT(receiveAuthToken()));
}


void LinTORManager::receiveAuthToken()
{
  QJson::Parser parser;

  bool ok;

  QVariantMap result = parser.parse(authReply->readAll(), &ok).toMap();

  authReply->deleteLater();
  authReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader authorization data";
    emit loginFailed();
    return;
  }

  authToken = result["Auth"].toString();

  authHeaderString = "GoogleLogin auth=";
  authHeaderString += authToken;

  qDebug() << "Authentication: " << authHeaderString;

  // Initialize the display:
  getFolders();
//  getUnread();
//  getSubscriptions();
}


void LinTORManager::getFolders()
{
  if (foldersReply)
  {
    // Already making a folders request.
    return;
  }

  if (!foldersRequest)
  {
    // Construct the request:
    foldersRequest = new QNetworkRequest(QUrl(
      "https://theoldreader.com/reader/api/0/tag/list?output=json"));

    foldersRequest->setRawHeader("Authorization", authHeaderString.toAscii());
  }

  foldersReply = qnam->get(*foldersRequest);

  connect(
    foldersReply,
    SIGNAL(finished()),
    this,
    SLOT(setupFolders()));
}


void LinTORManager::setupFolders()
{
  // Use the folders response to construct the flickableTabBar:
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(foldersReply->readAll(), &ok).toMap();

  foldersReply->deleteLater();
  foldersReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader folders data";
    return;
  }

  // Clear out any existing folder items:
  folderItems.clear();
  torTreeWidget->clear();

  QList<QVariant> folderVariants = result["tags"].toList();

  if (!folderVariants.isEmpty())
  {
    QList<QVariant>::const_iterator index = folderVariants.constBegin();
    QList<QVariant>::const_iterator end = folderVariants.constEnd();
    QMap<QString, QVariant> singleFolderMap;
    LinTORFolderItem *item;
    QString folderID;

    while (index != end)
    {
      singleFolderMap = (*index).toMap();

      folderID = singleFolderMap["id"].toString(),
      
      item = new LinTORFolderItem(
        folderID,
        singleFolderMap["sortid"].toString(),
        Standard_TORFolder);

      torTreeWidget->addTopLevelItem(item);

      folderItems[folderID] = item;

      ++index;
    }
  }

  // Add a "reading-list" item:
  allSubsItem =
    new LinTORFolderItem("user/-/state/com.google/reading-list", "", Standard_TORFolder);

  torTreeWidget->addTopLevelItem(allSubsItem);

  getSubscriptions();
}


/*
void LinTORManager::getUnread()
{
  if (unreadReply)
  {
    // Already making a subscriptions request.
    return;
  }

  if (!unreadRequest)
  {
    // Construct the request:
    unreadRequest = new QNetworkRequest(QUrl(
      "https://theoldreader.com/reader/api/0/unread?output=json"));

    unreadRequest->setRawHeader("Authorization", authHeaderString.toAscii());
  }

  unreadReply = qnam->get(*unreadRequest);

  connect(
    unreadReply,
    SIGNAL(finished()),
    this,
    SLOT(setupUnread()));
}


void LinTORManager::setupUnread()
{
  qDebug() << "unread\n";
  qDebug() << unreadReply->readAll();
  unreadReply->deleteLater();
  unreadReply = 0;
}
*/


void LinTORManager::getSubscriptions()
{
  if (subscriptionsReply)
  {
    // Already making a subscriptions request.
    return;
  }

  if (!subscriptionsRequest)
  {
    // Construct the request:
    subscriptionsRequest = new QNetworkRequest(QUrl(
      "https://theoldreader.com/reader/api/0/subscription/list?output=json"));

    subscriptionsRequest->setRawHeader(
      "Authorization", authHeaderString.toAscii());
  }

  subscriptionsReply = qnam->get(*subscriptionsRequest);

  connect(
    subscriptionsReply,
    SIGNAL(finished()),
    this,
    SLOT(setupSubscriptions()));
}


void LinTORManager::setupSubscriptions()
{
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(subscriptionsReply->readAll(), &ok).toMap();
  subscriptionsReply->deleteLater();
  subscriptionsReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader subscriptions data";
    return;
  }

  QList<QVariant> subscriptionsVariants = result["subscriptions"].toList();

  if (!subscriptionsVariants.isEmpty())
  {
    QList<QVariant>::const_iterator index = subscriptionsVariants.constBegin();
    QList<QVariant>::const_iterator end = subscriptionsVariants.constEnd();
    QMap<QString, QVariant> singleSubscriptionMap;
    LinTORSubscriptionItem *item;

    while (index != end)
    {
      singleSubscriptionMap = (*index).toMap();
      item = new LinTORSubscriptionItem(
        singleSubscriptionMap["id"].toString(),
        singleSubscriptionMap["title"].toString(),
        singleSubscriptionMap["sortid"].toString(),
        singleSubscriptionMap["firstitemmsec"].toString(),
        singleSubscriptionMap["url"].toString(),
        singleSubscriptionMap["htmlUrl"].toString(),
        singleSubscriptionMap["iconUrl"].toString(),
        qnam);

      // Make this a child of the All Subs item:
      allSubsItem->addChild(item);

      QList<QVariant> categoriesVariants =
        singleSubscriptionMap["categories"].toList();

      if (!categoriesVariants.isEmpty())
      {
        QList<QVariant>::const_iterator catsIndex =
          categoriesVariants.constBegin();
        QList<QVariant>::const_iterator catsEnd =
          categoriesVariants.constEnd();
        QMap<QString, QVariant> singleCatMap;

        while (catsIndex != catsEnd)
        {
          singleCatMap = (*catsIndex).toMap();
          QString catID = singleCatMap["id"].toString();
//          item->addCategory(catID, singleCatMap["label"].toString());

          if (folderItems.contains(catID))
          {
            folderItems[catID]->addChild(
              new LinTORSubscriptionItem(*item));
          }

          ++catsIndex;
        }
      }

      ++index;
    }
  }

  // Now, query for unread numbers:
  getUnread();
}


void LinTORManager::getUnread()
{
  if (unreadReply)
  {
    // Already making a unread request.
    return;
  }

  if (!unreadRequest)
  {
    // Construct the request:
    unreadRequest = new QNetworkRequest(QUrl(
      "https://theoldreader.com/reader/api/0/unread-count?output=json"));

    unreadRequest->setRawHeader(
      "Authorization", authHeaderString.toAscii());
  }

  unreadReply = qnam->get(*unreadRequest);

  connect(
    unreadReply,
    SIGNAL(finished()),
    this,
    SLOT(setupUnread()));
}


void LinTORManager::setupUnread()
{
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(unreadReply->readAll(), &ok).toMap();
  unreadReply->deleteLater();
  unreadReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader unread data";
    return;
  }

  QList<QVariant> unreadVariants = result["unreadcounts"].toList();

  if (!unreadVariants.isEmpty())
  {
    QList<QVariant>::const_iterator index = unreadVariants.constBegin();
    QList<QVariant>::const_iterator end = unreadVariants.constEnd();
    QMap<QString, QVariant> singleUnreadMap;
    QList<QTreeWidgetItem *> matchingFolders;
    LinTORFolderItem *folderItem;

    while (index != end)
    {
      singleUnreadMap = (*index).toMap();
      matchingFolders = torTreeWidget->findItems(
        singleUnreadMap["id"].toString(),
        Qt::MatchExactly);

      if (!matchingFolders.isEmpty())
      {
        QList<QTreeWidgetItem *>::const_iterator matchIndex =
          matchingFolders.constBegin();
        QList<QTreeWidgetItem *>::const_iterator matchEnd = 
          matchingFolders.constEnd();
        while (matchIndex != matchEnd)
        {
          folderItem = dynamic_cast<LinTORFolderItem *>(*matchIndex);
          if (folderItem)
          {
            folderItem->setUnreadCount(singleUnreadMap["count"].toInt());
          }
          ++matchIndex;
        }
      }

      ++index;
    }
  }
}


void LinTORManager::retrieveTORItems(
  QTreeWidgetItem *treeItem)
{
  if (itemIDsReply)
  {
    // Already making an item IDs request.
    return;
  }

  QString collectionID;

  LinTORFolderItem *tfi = dynamic_cast<LinTORFolderItem *>(treeItem);

  if (tfi)
  {
    collectionID = tfi->getID();
  }
  else
  {
    LinTORSubscriptionItem *tsi =
      dynamic_cast<LinTORSubscriptionItem *>(treeItem);

    if (!tsi)
    {
      // Unable to parse tree item, report an error.
      return;
    }

    collectionID = tsi->getID();
  }

  QString itemIDsRequestString =
    "https://theoldreader.com/reader/api/0/stream/items/ids?output=json";

  // Need to determine how many items to pull over maximum:
  itemIDsRequestString += "&n=500";

  itemIDsRequestString += "&s=";
  itemIDsRequestString += collectionID;

  QUrl qurl(itemIDsRequestString);

  QNetworkRequest itemIDsRequest(qurl);

  itemIDsRequest.setRawHeader(
    "Authorization", authHeaderString.toAscii());

  itemIDsReply = qnam->get(itemIDsRequest);

  connect(
    itemIDsReply,
    SIGNAL(finished()),
    this,
    SLOT(parseItemIDs()));
}


void LinTORManager::parseItemIDs()
{
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(itemIDsReply->readAll(), &ok).toMap();
  itemIDsReply->deleteLater();
  itemIDsReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader id list";
    return;
  }


  QList<QVariant> itemRefsVariants = result["itemRefs"].toList();

  torItems.clear();

  if (itemRefsVariants.isEmpty())
  {
    // Return an empty list:
    emit itemsReady();
    return;
  }

  QString itemRequestString =
    "https://theoldreader.com/reader/api/0/stream/items/contents?output=json";

  QList<QVariant>::const_iterator currentID = itemRefsVariants.constBegin();
  QList<QVariant>::const_iterator endOfIDs = itemRefsVariants.constEnd();

  while (currentID != endOfIDs)
  {
    QVariantMap itemIDMap = (*currentID).toMap();
    itemRequestString += "&i=";
    itemRequestString += itemIDMap["id"].toString();
    ++currentID;
  }

  if (torItemReply)
  {
    // Stop any existing request:
    torItemReply->abort();
    torItemReply = 0;
  }

  QUrl qurl(itemRequestString);

  QNetworkRequest itemRequest(qurl);

  itemRequest.setRawHeader(
    "Authorization", authHeaderString.toAscii());

  torItemReply = qnam->get(itemRequest);

  connect(
    torItemReply,
    SIGNAL(finished()),
    this,
    SLOT(collectItems()));
}


void LinTORManager::collectItems()
{
  // Marshall the current item:
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(torItemReply->readAll(), &ok).toMap();
  torItemReply->deleteLater();
  torItemReply = 0;

  if (!ok)
  {
    qWarning() << "Failed to parse The Old Reader item contents";
    return;
  }

  // Try to pull the usable bits out of the item contents:
  QList<QVariant> itemsVariants = result["items"].toList();

  QList<QVariant>::const_iterator index = itemsVariants.constBegin();
  QList<QVariant>::const_iterator end = itemsVariants.constEnd();

  while (index != end)
  {
    QVariantMap itemMap = (*index).toMap();

    LinTORItem item;

    item.setTitle(itemMap["title"].toString());

    QList<QVariant> canonicalList = itemMap["canonical"].toList();

    // For now, just pull the first item out of the list:
    if (!canonicalList.isEmpty())
    {
      QVariantMap canonicalMap = canonicalList.first().toMap();
      item.setSourceUrl(canonicalMap["href"].toString());
    }

    QList<QVariant> enclosureList = itemMap["enclosure"].toList();

    // For now, just pull the first item out of the list:
    if (!enclosureList.isEmpty())
    {
      QVariantMap enclosureMap = enclosureList.first().toMap();

      item.setEnclosureUrl(enclosureMap["href"].toString());
      item.setEnclosureType(enclosureMap["type"].toString());
    }

    QVariantMap summaryMap = itemMap["summary"].toMap();

    item.setSummary(summaryMap["content"].toString());

    torItems[itemMap["id"].toString()] = item;

    ++index;
  }

  emit itemsReady();
}


bool LinTORManager::hasTORItem(
  QString id)
{
  return torItems.contains(id);
}


LinTORItem LinTORManager::getTORItem(
  QString id)
{
  return torItems[id];
}


void LinTORManager::clearFolders()
{
  QMap<QString, LinTORFolderItem *>::iterator fiindex = folderItems.begin();
  QMap<QString, LinTORFolderItem *>::iterator fiend = folderItems.end();

  while (fiindex != fiend)
  {
    delete fiindex.value();
    fiindex.value() = 0;
    ++fiindex;
  }

  folderItems.clear();
}


void LinTORManager::markAsRead(
  QString id)
{
  QUrl marUrl("https://theoldreader.com/reader/api/0/edit-tag");
  QNetworkRequest marRequest(marUrl);
  marRequest.setRawHeader("Authorization", authHeaderString.toAscii());

  QString marPost = "i=";
  marPost += id;
  marPost += "&a=user/-/state/com.google/read";

  qnam->post(marRequest, marPost.toUtf8());

  // Do I need to check the reply?
}
