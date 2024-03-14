#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

QString PasswordDialog::getPassword() const
{
    return ui->lineEditPassword_3->text();
}

void PasswordDialog::on_pushButtonSend_3_clicked()
{
    QString password = ui->lineEditPassword_3->text();
    emit sendPassword(password);
}

void PasswordDialog::on_pushButtonCancel_3_clicked()
{
    // Fermer la fenêtre
    close();
}
