
#include <QApplication>
#include "StudentCheckInWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StudentCheckInWindow window;
    window.show();

    return app.exec();
}
