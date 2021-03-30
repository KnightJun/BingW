#pragma once
#include <QDialog>

class Ui_Dialog;
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AboutDialog();
public slots:
    void on_btnGithub_clicked();
private:
    Ui_Dialog *ui;
};
