#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QNetworkAccessManager>

class LinNowPlayingForm;
class LinVideoDisplayForm;
class QListWidgetItem;
class LinNewsfeedWidgetItem;
class QXmlStreamReader;
class LinDocumentationForm;
class LinAboutForm;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

private slots:
  void on_mediaListWidget_itemActivated(QListWidgetItem *item);

  void on_actionLoad_Newsfeeds_File_triggered();
  void on_actionDocumentation_triggered();
  void on_actionAbout_triggered();

  void on_frequencyComboBox_currentIndexChanged(int index);
  void on_categoryComboBox_currentIndexChanged(int index);

private:
  void retrieveNewsfeeds();

  void parseXML(
    QXmlStreamReader &reader);

  void parseLinguineElement(
    QXmlStreamReader &reader);

  QString parseNameElement(
    QXmlStreamReader &reader);

  QString parseText(
    QXmlStreamReader &reader,
    QString elementName);

  void filterItem(
    LinNewsfeedWidgetItem *nwi);

  void refilter();

  Ui::MainWindow *ui;

  LinNowPlayingForm *nowPlayingForm;
  LinVideoDisplayForm *videoDisplayForm;
  LinDocumentationForm *documentationForm;
  LinAboutForm *aboutForm;

  QNetworkAccessManager qnam;
};

#endif // MAINWINDOW_H
