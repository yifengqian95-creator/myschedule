#include <QApplication>
#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow mainWin(login.getUsername());
        mainWin.show();
        return app.exec();
    }

    return 0;
}
