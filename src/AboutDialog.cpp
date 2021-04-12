#include "AboutDialog.h"
#include <QDesktopServices>
#include <qurl.h>
#include <ui_AboutDialog.h>

AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    ui = new Ui_Dialog();
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::on_btnGithub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/knightjun/BingW"));
}