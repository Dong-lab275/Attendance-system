
#include "StudentCheckInWindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

StudentCheckInWindow::StudentCheckInWindow(QWidget *parent)
    : QWidget(parent) {

    setWindowTitle("Student Check-in");
    resize(400, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("STUDENT CHECK-IN");
    QLabel *sessionLabel = new QLabel("Session ID:");

    QLineEdit *sessionInput = new QLineEdit();
    sessionInput->setPlaceholderText("Enter Session ID");

    QLabel *statusLabel = new QLabel("Session Status: Completed");

    QPushButton *checkInButton = new QPushButton("Check In");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(sessionLabel);
    mainLayout->addWidget(sessionInput);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(checkInButton);

    mainLayout->addStretch();

    connect(checkInButton, &QPushButton::clicked,
            this, [this, sessionInput]() {

        if (sessionInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(
                this,
                "Warning",
                "Please enter Session ID!"
            );
            return;
        }

        QMessageBox::warning(
            this,
            "Check-in Denied",
            "Session is closed!"
        );
    });
}
