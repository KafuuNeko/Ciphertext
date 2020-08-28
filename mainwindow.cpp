#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "passworddialog.h"
#include "tea.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    tea::Bytes bytes({255, 22, 33, 44, 55, 66, 255, 255});
//    uint64_t value = tea::bytesToInt64(bytes, 0);
//    tea::Bytes rec(8);
//    tea::int64ToBytes(value, rec, 0);

//    qDebug() << value;

//    for(int i = 0; i < 8; ++i)
//    {
//        qDebug() << rec.get()[i];
//    }

    initTextEditor();

}

void MainWindow::initTextEditor()
{
    connect(ui->textSelect, &QToolButton::clicked, [=](){ textOpen(); });
    connect(ui->textSave, &QToolButton::clicked, [=](){ textSave(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::textOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择要打开的文件", "", "*.cip");
    if(fileName.isEmpty()) return;
    ui->textPath->setText(fileName);

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray bytes = file.readAll();
    tea::Bytes data = tea::Bytes(reinterpret_cast<tea::byte*>(bytes.data()), bytes.size());
    file.close();

    PasswordDialog passwordDialog(this, this->textPassword_);
    bool decrpy_flag = false;
    do
    {
        passwordDialog.isCancel_ = true;
        passwordDialog.password_ = this->textPassword_;
        passwordDialog.exec();
        if(passwordDialog.isCancel_) break;

        this->textPassword_ = passwordDialog.password_;

        QString result = tea::decrpy_string(data,tea::Key(this->textPassword_.toStdString()), 32, &decrpy_flag).c_str();
        if(decrpy_flag)
            ui->textContent->setText(result);
        else
            QMessageBox::critical(this, "Error:", "解密失败，可能密码错误，请重试");
    } while(!decrpy_flag);

    data.reset();
}

void MainWindow::textSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, "选择保存路径", ui->textPath->text(), "*.cip");
    if(fileName.isEmpty()) return;

    PasswordDialog passwordDialog(this, this->textPassword_);
    passwordDialog.exec();
    if(passwordDialog.isCancel_) return;
    this->textPassword_ = passwordDialog.password_;

    ui->textPath->setText(fileName);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    tea::Bytes bytes = tea::encrypt_string(ui->textContent->toHtml().toStdString(), tea::Key(this->textPassword_.toStdString()));
    file.write(reinterpret_cast<char*>(bytes.get()), bytes.size());
    bytes.reset();
    QMessageBox::information(this, "信息：", "保存成功");
    file.close();
}
