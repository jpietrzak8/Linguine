#include "linfeedselectordialog.h"
#include "ui_linfeedselectordialog.h"

#include "linfeedsource.h"
#include <QListWidgetItem>

LinFeedSelectorDialog::LinFeedSelectorDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LinFeedSelectorDialog)
{
  ui->setupUi(this);

  ui->sourcesListWidget->addItem(
    new LinNativeFeedSource());

  ui->sourcesListWidget->addItem(
    new LinTORFeedSource());
}

LinFeedSelectorDialog::~LinFeedSelectorDialog()
{
  delete ui;
}

void LinFeedSelectorDialog::on_sourcesListWidget_itemActivated(QListWidgetItem *item)
{
  LinFeedSource *fs = dynamic_cast<LinFeedSource *>(item);

  switch (fs->getType())
  {
  case TheOldReader_Source:
    emit torChosen();
    break;

  case Native_Source:
  default:
    emit nativeChosen();
    break;
  }

  accept();
}
