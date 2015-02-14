#ifndef LINTORMANAGER_H
#define LINTORMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QMap>
#include <QVariant>
#include "lintoritems.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QTreeWidget;
class QTreeWidgetItem;

class LinTORManager: public QObject
{
  Q_OBJECT

public:
  LinTORManager(
    QNetworkAccessManager *qnam,
    QTreeWidget *qtw);

  ~LinTORManager();

  void getFolders();

//  void getUnread();

  void getSubscriptions();

  void getUnread();

  void retrieveTORItems(
    QTreeWidgetItem *treeItem);

  const LinTORItemCollection getTORItemCollection()
    { return torItems; }

  bool hasTORItem(
    QString id);

  LinTORItem getTORItem(
    QString id);

  void authenticate(
    QString email,
    QString passwd);

  void markAsRead(
    QString id);

signals:
  void loginFailed();
  void itemsReady();

private slots:
  void receiveAuthToken();

  void setupFolders();
//  void setupUnread();
  void setupSubscriptions();
  void setupUnread();

  void parseItemIDs();
  void collectItems();

private:
  void clearFolders();

  QTreeWidget *torTreeWidget;
  LinTORFolderItem *allSubsItem;

  QNetworkAccessManager *qnam;

  QString authToken;
  QString authHeaderString;
  QNetworkReply *authReply;

  QNetworkRequest *foldersRequest;
  QNetworkReply *foldersReply;
  QMap<QString, LinTORFolderItem *> folderItems;

  QNetworkRequest *subscriptionsRequest;
  QNetworkReply *subscriptionsReply;

  QNetworkRequest *unreadRequest;
  QNetworkReply *unreadReply;

  QNetworkReply *itemIDsReply;

  QNetworkReply *torItemReply;
  LinTORItemCollection torItems;
};

#endif // LINTORMANAGER_H
