#include "fourthbio.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

FourthBio::FourthBio(const QString &userEmail, QWidget *parent)
    : QWidget(parent), currentUserEmail(userEmail)
{
    setupUI();
}

void FourthBio::setupUI()
{
    // Set window properties
    setWindowTitle("FitTrack - Dietary Restrictions");
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
    subtitleLabel = new QLabel("Select The Dietary Restrictions You want to Include");
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

    // Instructions
    instructionsLabel = new QLabel("Select The options you want to exclude from your diet due to any medical concern or personal dislike");
    instructionsLabel->setStyleSheet(R"(
        QLabel {
            font-size: 12px;
            color: rgba(255, 255, 255, 0.7);
            margin-bottom: 20px;
            line-height: 1.4;
        }
    )");
    instructionsLabel->setWordWrap(true);
    modalLayout->addWidget(instructionsLabel);
/*
    // Create checkbox styling
    QString checkboxStyle = R"(
        QCheckBox {
            font-size: 14px;
            color: #ffffff;
            padding: 12px 16px;
            margin: 5px 0;
            background-color: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            spacing: 10px;
        }
        QCheckBox:hover {
            background-color: rgba(255, 255, 255, 0.2);
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 3px;
            border: 2px solid rgba(255, 255, 255, 0.7);
            background-color: transparent;
        }
        QCheckBox::indicator:checked {
            background-color: #4CAF50;
            border-color: #4CAF50;
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTQiIGhlaWdodD0iMTEiIHZpZXdCb3g9IjAgMCAxNCAxMSIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEuNSA1LjVMNSA5TDEyLjUgMS41IiBzdHJva2U9IndoaXRlIiBzdHJva2Utd2lkdGg9IjIiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIvPgo8L3N2Zz4K);
        }
    )";

    // Create dietary restriction checkboxes
    nightshadesCheckbox = new QCheckBox("A Nightshades");
    nightshadesCheckbox->setStyleSheet(checkboxStyle);

    glutenCheckbox = new QCheckBox("A Gluten");
    glutenCheckbox->setStyleSheet(checkboxStyle);

    highCholesterolCheckbox = new QCheckBox("A High Cholesterol Foods");
    highCholesterolCheckbox->setStyleSheet(checkboxStyle);

    eggsCheckbox = new QCheckBox("A Eggs");
    eggsCheckbox->setStyleSheet(checkboxStyle);

    highSugarCheckbox = new QCheckBox("B High Sugar Foods");
    highSugarCheckbox->setStyleSheet(checkboxStyle);

    alcoholCheckbox = new QCheckBox("A Alcohol");
    alcoholCheckbox->setStyleSheet(checkboxStyle);

    highSodiumCheckbox = new QCheckBox("A High Sodium (Salts) Foods");
    highSodiumCheckbox->setStyleSheet(checkboxStyle);

    highSaturatedFatsCheckbox = new QCheckBox("A High Saturated (Fats) Foods");
    highSaturatedFatsCheckbox->setStyleSheet(checkboxStyle);

    caffeinatedDrinksCheckbox = new QCheckBox("A Caffeinated Drinks");
    caffeinatedDrinksCheckbox->setStyleSheet(checkboxStyle);

    redMeatCheckbox = new QCheckBox("A Red Meat");
    redMeatCheckbox->setStyleSheet(checkboxStyle);

    // Add checkboxes to layout
    modalLayout->addWidget(nightshadesCheckbox);
    modalLayout->addWidget(glutenCheckbox);
    modalLayout->addWidget(highCholesterolCheckbox);
    modalLayout->addWidget(eggsCheckbox);
    modalLayout->addWidget(highSugarCheckbox);
    modalLayout->addWidget(alcoholCheckbox);
    modalLayout->addWidget(highSodiumCheckbox);
    modalLayout->addWidget(highSaturatedFatsCheckbox);
    modalLayout->addWidget(caffeinatedDrinksCheckbox);
    modalLayout->addWidget(redMeatCheckbox);

    // Add spacing before button
    modalLayout->addItem(new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));
*/// ... [previous code until after instructionsLabel] ...

    // Create a horizontal layout for the two columns
    QHBoxLayout *columnsLayout = new QHBoxLayout();
    columnsLayout->setSpacing(20); // Space between columns

    // Create left and right column layouts
    QVBoxLayout *leftColumn = new QVBoxLayout();
    QVBoxLayout *rightColumn = new QVBoxLayout();
    leftColumn->setSpacing(5);
    rightColumn->setSpacing(5);

    // Create checkbox styling (keep your existing style)
    QString checkboxStyle = R"(
    QCheckBox {
        font-size: 14px;
        color: #ffffff;
        padding: 12px 16px;
        margin: 5px 0;
        background-color: rgba(255, 255, 255, 0.1);
        border-radius: 8px;
        spacing: 10px;
    }
    QCheckBox:hover {
        background-color: rgba(255, 255, 255, 0.2);
    }
    QCheckBox::indicator {
        width: 18px;
        height: 18px;
        border-radius: 3px;
        border: 2px solid rgba(255, 255, 255, 0.7);
        background-color: transparent;
    }
    QCheckBox::indicator:checked {
        background-color: #4CAF50;
        border-color: #4CAF50;
        image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTQiIGhlaWdodD0iMTEiIHZpZXdCb3g9IjAgMCAxNCAxMSIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEuNSA1LjVMNSA5TDEyLjUgMS41IiBzdHJva2U9IndoaXRlIiBzdHJva2Utd2lkdGg9IjIiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIvPgo8L3N2Zz4K);
    }
)";

    // Create checkboxes and add to columns (distributed evenly)
    nightshadesCheckbox = new QCheckBox("A Nightshades");
    nightshadesCheckbox->setStyleSheet(checkboxStyle);
    leftColumn->addWidget(nightshadesCheckbox);

    glutenCheckbox = new QCheckBox("B Gluten");
    glutenCheckbox->setStyleSheet(checkboxStyle);
    leftColumn->addWidget(glutenCheckbox);

    highCholesterolCheckbox = new QCheckBox("C High Cholesterol Foods");
    highCholesterolCheckbox->setStyleSheet(checkboxStyle);
    leftColumn->addWidget(highCholesterolCheckbox);

    eggsCheckbox = new QCheckBox("D Eggs");
    eggsCheckbox->setStyleSheet(checkboxStyle);
    leftColumn->addWidget(eggsCheckbox);

    highSugarCheckbox = new QCheckBox("E High Sugar Foods");
    highSugarCheckbox->setStyleSheet(checkboxStyle);
    leftColumn->addWidget(highSugarCheckbox);

    alcoholCheckbox = new QCheckBox("F Alcohol");
    alcoholCheckbox->setStyleSheet(checkboxStyle);
    rightColumn->addWidget(alcoholCheckbox);

    highSodiumCheckbox = new QCheckBox("G High Sodium (Salts) Foods");
    highSodiumCheckbox->setStyleSheet(checkboxStyle);
    rightColumn->addWidget(highSodiumCheckbox);

    highSaturatedFatsCheckbox = new QCheckBox("H High Saturated (Fats) Foods");
    highSaturatedFatsCheckbox->setStyleSheet(checkboxStyle);
    rightColumn->addWidget(highSaturatedFatsCheckbox);

    caffeinatedDrinksCheckbox = new QCheckBox("I Caffeinated Drinks");
    caffeinatedDrinksCheckbox->setStyleSheet(checkboxStyle);
    rightColumn->addWidget(caffeinatedDrinksCheckbox);

    redMeatCheckbox = new QCheckBox("J Red Meat");
    redMeatCheckbox->setStyleSheet(checkboxStyle);
    rightColumn->addWidget(redMeatCheckbox);

    // Add stretch to both columns to push content to top
    leftColumn->addStretch();
    rightColumn->addStretch();

    // Add columns to the horizontal layout
    columnsLayout->addLayout(leftColumn);
    columnsLayout->addLayout(rightColumn);

    // Add the columns layout to the main modal layout
    modalLayout->addLayout(columnsLayout);

    // Add spacing before button
    modalLayout->addItem(new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // ... [rest of your existing code] ...
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
    connect(nextButton, &QPushButton::clicked, this, &FourthBio::handleNextStep);
}

void FourthBio::resizeEvent(QResizeEvent *event)
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

void FourthBio::setupBackgroundImage()
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

void FourthBio::showFourthBioPage()
{
    // Show the fourth biodata form
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

void FourthBio::handleNextStep()
{
    // Collect selected dietary restrictions
    QStringList restrictions;

    if (nightshadesCheckbox->isChecked()) restrictions << "Nightshades";
    if (glutenCheckbox->isChecked()) restrictions << "Gluten";
    if (highCholesterolCheckbox->isChecked()) restrictions << "High Cholesterol Foods";
    if (eggsCheckbox->isChecked()) restrictions << "Eggs";
    if (highSugarCheckbox->isChecked()) restrictions << "High Sugar Foods";
    if (alcoholCheckbox->isChecked()) restrictions << "Alcohol";
    if (highSodiumCheckbox->isChecked()) restrictions << "High Sodium (Salts) Foods";
    if (highSaturatedFatsCheckbox->isChecked()) restrictions << "High Saturated (Fats) Foods";
    if (caffeinatedDrinksCheckbox->isChecked()) restrictions << "Caffeinated Drinks";
    if (redMeatCheckbox->isChecked()) restrictions << "Red Meat";

    // Validate that at least one restriction is selected
    if (restrictions.isEmpty()) {
        QMessageBox::warning(this, "No Restrictions Selected",
                             "Please select at least one dietary restriction or click 'Next step' if you have none.");
        return;
    }

    // Save the dietary restrictions
    if (saveDietaryRestrictions()) {
        QMessageBox::information(this, "Success", "Dietary restrictions saved successfully!");
        // TODO: Navigate to main dashboard or next screen
        this->hide();
    } else {
        QMessageBox::critical(this, "Error", "Failed to save your dietary restrictions. Please try again.");
    }
}

bool FourthBio::saveDietaryRestrictions()
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

            // Add dietary restrictions
            QJsonArray restrictionsArray;

            if (nightshadesCheckbox->isChecked()) restrictionsArray.append("Nightshades");
            if (glutenCheckbox->isChecked()) restrictionsArray.append("Gluten");
            if (highCholesterolCheckbox->isChecked()) restrictionsArray.append("High Cholesterol Foods");
            if (eggsCheckbox->isChecked()) restrictionsArray.append("Eggs");
            if (highSugarCheckbox->isChecked()) restrictionsArray.append("High Sugar Foods");
            if (alcoholCheckbox->isChecked()) restrictionsArray.append("Alcohol");
            if (highSodiumCheckbox->isChecked()) restrictionsArray.append("High Sodium (Salts) Foods");
            if (highSaturatedFatsCheckbox->isChecked()) restrictionsArray.append("High Saturated (Fats) Foods");
            if (caffeinatedDrinksCheckbox->isChecked()) restrictionsArray.append("Caffeinated Drinks");
            if (redMeatCheckbox->isChecked()) restrictionsArray.append("Red Meat");

            bioData["dietaryRestrictions"] = restrictionsArray;

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

QJsonDocument FourthBio::loadUserData()
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

bool FourthBio::updateUserData(const QJsonObject &newData)
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
