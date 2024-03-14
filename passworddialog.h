#pragma once

#include <QDialog>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget* parent = nullptr);
    ~PasswordDialog();

    QString getPassword() const;

signals:
    void sendPassword(const QString& password);

private slots:
    void on_pushButtonSend_3_clicked();
    void on_pushButtonCancel_3_clicked();

private:
    Ui::PasswordDialog* ui;
};
