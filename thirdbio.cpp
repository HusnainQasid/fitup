#include "thirdbio.h"
#include"fourthbio.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

ThirdBio::ThirdBio(const QString &userEmail, const QString &selectedGoal, QWidget *parent)
    : QWidget(parent), currentUserEmail(userEmail), fitnessGoal(selectedGoal)
{
    setupUI();
}

void ThirdBio::setupUI()
{
    // Set window properties
    setWindowTitle("FitTrack - Choose Time Frame");
    setWindowState(Qt::WindowMaximized);

    // Make window responsive to screen size
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    resize(screenGeometry.size());

    // Create background with gym image effect
    setupBackgroundImage();

    // Create the modal frame (translucent overlay)
    modalFrame = new QFrame(this);
    modalFrame->setMinimumSize(600, 700);
    modalFrame->setMaximumSize(700, 800);
    modalFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    modalFrame->setStyleSheet(R"(
        QFrame {
            background-color: rgba(255, 255, 255, 0.15);
            border-radius: 20px;
            backdrop-filter: blur(10px);
        }
    )");

    // Position modal in center
    int verticalPadding = height() * 0.2;
    int modalY = verticalPadding + (height() - 2 * verticalPadding - modalFrame->height()) / 2;
    modalFrame->move((width() - modalFrame->width()) / 2, modalY);

    // Setup modal layout
    modalLayout = new QVBoxLayout(modalFrame);
    modalLayout->setContentsMargins(50, 40, 50, 40);
    modalLayout->setSpacing(20);

    // Title
    titleLabel = new QLabel("Sweat Now, Shine Later");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 28px;
            font-weight: bold;
            color: #ffffff;
            margin-bottom: 10px;
        }
    )");
    titleLabel->setWordWrap(true);

    // Subtitle
    subtitleLabel = new QLabel("Choose The Time Frame You Want To Transform Yourself In.");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 14px;
            color: rgba(255, 255, 255, 0.8);
            margin-bottom: 20px;
            line-height: 1.4;
        }
    )");
    subtitleLabel->setWordWrap(true);

    modalLayout->addWidget(titleLabel);
    modalLayout->addWidget(subtitleLabel);

    // Add spacing
    modalLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Note section
    noteLabel = new QLabel("Note that:");
    noteLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #ffffff;
            margin-bottom: 10px;
        }
    )");
    modalLayout->addWidget(noteLabel);

    noteDescLabel = new QLabel("More faster you want to achieve your goal, More harder will be Exercises and Diet plan.");
    noteDescLabel->setStyleSheet(R"(
        QLabel {
            font-size: 12px;
            color: rgba(255, 255, 255, 0.7);
            margin-bottom: 20px;
            line-height: 1.4;
        }
    )");
    noteDescLabel->setWordWrap(true);
    modalLayout->addWidget(noteDescLabel);

    // Create radio button styling
    QString radioStyle = R"(
        QRadioButton {
            font-size: 14px;
            color: #ffffff;
            padding: 12px 16px;
            margin: 5px 0;
            background-color: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            spacing: 10px;
        }
        QRadioButton:hover {
            background-color: rgba(255, 255, 255, 0.2);
        }
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
            border-radius: 9px;
            border: 2px solid rgba(255, 255, 255, 0.7);
            background-color: transparent;
        }
        QRadioButton::indicator:checked {
            background-color: #4CAF50;
            border-color: #4CAF50;
        }
    )";

    // Create button group to ensure only one selection
    timeFrameButtonGroup = new QButtonGroup(this);

    // Create radio buttons for time frames
    twoMonthsRadio = new QRadioButton("A    2 months");
    twoMonthsRadio->setStyleSheet(radioStyle);
    timeFrameButtonGroup->addButton(twoMonthsRadio);

    fourMonthsRadio = new QRadioButton("A    4 months");
    fourMonthsRadio->setStyleSheet(radioStyle);
    timeFrameButtonGroup->addButton(fourMonthsRadio);

    sixMonthsRadio = new QRadioButton("A    6 months");
    sixMonthsRadio->setStyleSheet(radioStyle);
    timeFrameButtonGroup->addButton(sixMonthsRadio);

    moreThanSixRadio = new QRadioButton("D    More than 6 months");
    moreThanSixRadio->setStyleSheet(radioStyle);
    timeFrameButtonGroup->addButton(moreThanSixRadio);

    // Add radio buttons to layout
    modalLayout->addWidget(twoMonthsRadio);
    modalLayout->addWidget(fourMonthsRadio);
    modalLayout->addWidget(sixMonthsRadio);
    modalLayout->addWidget(moreThanSixRadio);

    // Add spacing before button
    modalLayout->addItem(new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Next step button
    nextButton = new QPushButton("Next step");
    nextButton->setMinimumHeight(50);
    nextButton->setCursor(Qt::PointingHandCursor);
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: #000000;
            color: white;
            border: none;
            border-radius: 25px;
            font-size: 16px;
            font-weight: 600;
            padding: 15px 40px;
        }
        QPushButton:hover {
            background-color: #333333;
        }
        QPushButton:pressed {
            background-color: #555555;
        }
    )");

    modalLayout->addWidget(nextButton);

    // Add silhouette image on the right side of the modal
    silhouetteLabel = new QLabel(this);
    silhouetteLabel->setFixedSize(150, 150);

    // Create silhouette placeholder
    silhouetteLabel->setText("🏃‍♂️🏃‍♀️");
    silhouetteLabel->setAlignment(Qt::AlignCenter);
    silhouetteLabel->setStyleSheet(R"(
        QLabel {
            font-size: 48px;
            color: rgba(255, 255, 255, 0.9);
            background-color: transparent;
        }
    )");

    // Position silhouette to the right of the modal
    int silhouetteX = modalFrame->x() + modalFrame->width() + 20;
    int silhouetteY = modalFrame->y() + modalFrame->height() - 200;
    silhouetteLabel->move(silhouetteX, silhouetteY);

    // Connect button
    connect(nextButton, &QPushButton::clicked, this, &ThirdBio::handleNextStep);
}

void ThirdBio::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Resize background
    backgroundLabel->resize(this->size());

    // Center modalFrame with vertical padding
    int verticalPadding = height() * 0.2;
    int modalY = verticalPadding + (height() - 2 * verticalPadding - modalFrame->height()) / 2;
    modalFrame->move((width() - modalFrame->width()) / 2, modalY);

    // Reposition silhouette
    int silhouetteX = modalFrame->x() + modalFrame->width() + 20;
    int silhouetteY = modalFrame->y() + modalFrame->height() - 200;
    silhouetteLabel->move(silhouetteX, silhouetteY);
}

void ThirdBio::setupBackgroundImage()
{
    // Create background label to fill entire window
    backgroundLabel = new QLabel(this);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower(); // Send to back

    // Load gym background image
    QPixmap gymBackground("images/gym.jpg");
    if (!gymBackground.isNull()) {
        // Scale image to fit window while keeping aspect ratio
        gymBackground = gymBackground.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        // Optionally darken the image
        QPainter painter(&gymBackground);
        painter.fillRect(gymBackground.rect(), QColor(0, 0, 0, 120)); // Dark overlay

        // Set image to background label
        backgroundLabel->setPixmap(gymBackground);
        backgroundLabel->setScaledContents(true);
    } else {
        // Fallback: Use gradient background
        backgroundLabel->setStyleSheet(R"(
            QLabel {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                            stop:0 #1a1a1a,
                            stop:0.3 #2d2d2d,
                            stop:0.7 #1a1a1a,
                            stop:1 #0d0d0d);
            }
        )");
    }
}

void ThirdBio::showThirdBioPage()
{
    // Show the third biodata form
    this->show();
    this->raise();
    this->activateWindow();

    // Ensure modal is centered after window is shown
    int verticalPadding = height() * 0.15;
    int modalY = verticalPadding + (height() - 2 * verticalPadding - modalFrame->height()) / 2;
    modalFrame->move((width() - modalFrame->width()) / 2, modalY);

    // Reposition silhouette
    int silhouetteX = modalFrame->x() + modalFrame->width() + 20;
    int silhouetteY = modalFrame->y() + modalFrame->height() - 200;
    silhouetteLabel->move(silhouetteX, silhouetteY);
}

void ThirdBio::handleNextStep()
{
    // Collect selected time frame
    QString selectedTimeFrame;

    if (twoMonthsRadio->isChecked()) {
        selectedTimeFrame = "2 months";
    } else if (fourMonthsRadio->isChecked()) {
        selectedTimeFrame = "4 months";
    } else if (sixMonthsRadio->isChecked()) {
        selectedTimeFrame = "6 months";
    } else if (moreThanSixRadio->isChecked()) {
        selectedTimeFrame = "More than 6 months";
    }

    // Validate that a time frame is selected
    if (selectedTimeFrame.isEmpty()) {
        QMessageBox::warning(this, "No Time Frame Selected",
                             "Please select a time frame for your fitness goal.");
        return;
    }

    // Save the time frame data
    if (saveTimeFrameData()) {
       /* // Show success message
        QString message = QString("Time frame selected successfully!\n\n"
                                  "Goal: %1\n"
                                  "Time Frame: %2\n\n"
                                  "Your fitness journey setup is complete!")
                              .arg(fitnessGoal, selectedTimeFrame);

        QMessageBox::information(this, "Setup Complete", message);
*/
        FourthBio *fourthBioPage = new FourthBio(currentUserEmail);
        fourthBioPage->showFourthBioPage();

        // TODO: Navigate to main dashboard or next screen
        this->hide();
    } else {
        QMessageBox::critical(this, "Error", "Failed to save your time frame data. Please try again.");
    }
}

bool ThirdBio::saveTimeFrameData()
{
    QJsonDocument userData = loadUserData();

    if (userData.isNull()) {
        qDebug() << "Failed to load user data";
        return false;
    }

    QJsonObject userDataObj = userData.object();
    QJsonArray users = userDataObj["users"].toArray();

    // Find the current user and update their data
    for (int i = 0; i < users.size(); ++i) {
        QJsonObject user = users[i].toObject();
        if (user["email"].toString() == currentUserEmail) {
            // Get existing bioData or create new one
            QJsonObject bioData = user["bioData"].toObject();

            // Add time frame data
            QString timeFrame;
            if (twoMonthsRadio->isChecked()) {
                timeFrame = "2 months";
            } else if (fourMonthsRadio->isChecked()) {
                timeFrame = "4 months";
            } else if (sixMonthsRadio->isChecked()) {
                timeFrame = "6 months";
            } else if (moreThanSixRadio->isChecked()) {
                timeFrame = "More than 6 months";
            }

            bioData["fitnessGoal"] = fitnessGoal;
            bioData["timeFrame"] = timeFrame;

            // Update user object
            user["bioData"] = bioData;
            users[i] = user;

            // Update the users array in the main object
            userDataObj["users"] = users;

            // Save back to file
            return updateUserData(userDataObj);
        }
    }

    qDebug() << "User not found:" << currentUserEmail;
    return false;
}

QJsonDocument ThirdBio::loadUserData()
{
    QDir dataDir(QDir::currentPath() + "/data");
    QFile file(dataDir.filePath("users.json"));

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open users.json file:" << file.errorString();
        return QJsonDocument();
    }

    QByteArray userData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(userData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return QJsonDocument();
    }

    return doc;
}

bool ThirdBio::updateUserData(const QJsonObject &newData)
{
    QDir dataDir(QDir::currentPath() + "/data");
    QFile file(dataDir.filePath("users.json"));

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open users.json file for writing:" << file.errorString();
        return false;
    }

    QJsonDocument doc(newData);
    file.write(doc.toJson());
    file.close();

    qDebug() << "User data updated successfully for:" << currentUserEmail;
    return true;
}
