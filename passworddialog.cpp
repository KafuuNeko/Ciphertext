#include "passworddialog.h"
#include "ui_passworddialog.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

PasswordDialog::PasswordDialog(QWidget *parent, QString defaultPassword) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    password_ = defaultPassword;
    ui->password->setText(password_);
    connect(ui->cancel, &QToolButton::clicked, [=]() {
        this->close();
    });

    connect(ui->confirm, &QToolButton::clicked, [=]() {
        password_ = ui->password->text();
        if(password_.isEmpty())
        {
            QMessageBox::warning(this, "警告：", "密码不能为空！");
            return;
        }

        isCancel_ = false;
        this->close();
    });

}

void PasswordDialog::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
    {
        emit ui->confirm->clicked();
    }
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}
