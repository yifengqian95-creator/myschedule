#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
    Q_OBJECT

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginBtn;
    QPushButton *cancelBtn;

    QString username;

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getUsername() const { return username; }

private slots:
    void handleLogin();
};

#endif // LOGIN_DIALOG_H
