#include "LoginDialog.h"
#include "UserEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("MySchedule - 用户登录");
    resize(320, 180);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 用户名输入
    QHBoxLayout *userLayout = new QHBoxLayout();
    userLayout->addWidget(new QLabel("用户名:"));
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("请输入用户名");
    userLayout->addWidget(usernameEdit);

    // 密码输入
    QHBoxLayout *pwdLayout = new QHBoxLayout();
    pwdLayout->addWidget(new QLabel("密  码:"));
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("请输入密码");
    pwdLayout->addWidget(passwordEdit);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout();
    loginBtn = new QPushButton("登录 / 自动注册");
    cancelBtn = new QPushButton("取消");
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(pwdLayout);
    mainLayout->addLayout(btnLayout);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginDialog::handleLogin() {
    QString user = usernameEdit->text().trimmed();
    QString pwd = passwordEdit->text();

    if (user.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空！");
        return;
    }

    if (UserEngine::authenticateOrRegister(user.toStdString(), pwd.toStdString())) {
        username = user;
        accept(); // 关闭对话框并返回 QDialog::Accepted
    } else {
        QMessageBox::critical(this, "错误", "口令错误，登录失败！");
    }
}
