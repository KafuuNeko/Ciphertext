#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    QString password_;
    bool isCancel_ = true;

    explicit PasswordDialog(QWidget *parent = nullptr, QString defaultPassword = "");
    ~PasswordDialog();

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    Ui::PasswordDialog *ui;

};

#endif // PASSWORDDIALOG_H
