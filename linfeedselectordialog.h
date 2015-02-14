#ifndef LINFEEDSELECTORDIALOG_H
#define LINFEEDSELECTORDIALOG_H

#include <QDialog>

class QListWidgetItem;

namespace Ui {
class LinFeedSelectorDialog;
}

class LinFeedSelectorDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinFeedSelectorDialog(QWidget *parent = 0);
  ~LinFeedSelectorDialog();

signals:
  void torChosen();
  void nativeChosen();
  
private slots:
  void on_sourcesListWidget_itemActivated(QListWidgetItem *item);

private:
  Ui::LinFeedSelectorDialog *ui;
};

#endif // LINFEEDSELECTORDIALOG_H
