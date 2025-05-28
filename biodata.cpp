#include "biodata.h"
#include "secondbio.h"  // Add this include
#include"thirdbio.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

Biodata::Biodata(const QString &userEmail, QWidget *parent) : QWidget(parent), currentUserEmail(userEmail)
{
    setupUI();
}

void Biodata::setupUI()
{
    // Set window properties
    setWindowTitle("FitTrack - Personal Information");
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
    modalFrame->setMaximumSize(800, 900); // Optional: restrict upper size
    modalFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    modalFrame->setStyleSheet(R"(
        QFrame {
            background-color: rgba(255, 255, 255, 0.15);
            border-radius: 20px;
            backdrop-filter: blur(10px);
        }
    )");
    setMinimumSize(modalFrame->size());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // Position modal in center
    int verticalPadding = height() * 0.1; // 10% padding top and bottom
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
    subtitleLabel = new QLabel("The data you enter will help us to create the plan you want to achieve your desired goal.");
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

    // Age input
    ageLabel = new QLabel("Age");
    ageLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #ffffff;
            margin-bottom: 5px;
        }
    )");

    ageEdit = new QLineEdit();
    ageEdit->setPlaceholderText("Age");
    ageEdit->setMinimumHeight(35);
    ageEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.9);
            border: none;
            border-radius: 8px;
            padding: 12px 16px;
            font-size: 12px;
            color: #333333;
        }
        QLineEdit:focus {
            background-color: rgba(255, 255, 255, 1.0);
            outline: none;
        }
        QLineEdit::placeholder {
            color: #999999;
        }
    )");

    modalLayout->addWidget(ageLabel);
    modalLayout->addWidget(ageEdit);

    // Weight input
    weightLabel = new QLabel("Weight");
    weightLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #ffffff;
            margin-bottom: 5px;
        }
    )");

    weightEdit = new QLineEdit();
    weightEdit->setPlaceholderText("weight");
    weightEdit->setMinimumHeight(35);
    weightEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.9);
            border: none;
            border-radius: 8px;
            padding: 12px 16px;
            font-size: 12px;
            color: #333333;
        }
        QLineEdit:focus {
            background-color: rgba(255, 255, 255, 1.0);
            outline: none;
        }
        QLineEdit::placeholder {
            color: #999999;
        }
    )");

    modalLayout->addWidget(weightLabel);
    modalLayout->addWidget(weightEdit);

    // Height input
    heightLabel = new QLabel("Height");
    heightLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #ffffff;
            margin-bottom: 5px;
        }
    )");

    heightEdit = new QLineEdit();
    heightEdit->setPlaceholderText("height");
    heightEdit->setMinimumHeight(35);
    heightEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.9);
            border: none;
            border-radius: 8px;
            padding: 12px 16px;
            font-size: 12px;
            color: #333333;
        }
        QLineEdit:focus {
            background-color: rgba(255, 255, 255, 1.0);
            outline: none;
        }
        QLineEdit::placeholder {
            color: #999999;
        }
    )");

    modalLayout->addWidget(heightLabel);
    modalLayout->addWidget(heightEdit);

    // Add spacing before button
    modalLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

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
    silhouetteLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(0, 0, 0, 0.8);
            border-radius: 10px;
        }
    )");
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
    connect(nextButton, &QPushButton::clicked, this, &Biodata::handleNextStep);
}
void Biodata::saveBiodataToFile(int age, double weight, double height)
{
    QDir dataDir(QDir::currentPath() + "/data");
    QFile file(dataDir.filePath("users.json"));

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open users.json for reading";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject rootObj = doc.object();
    QJsonArray usersArray = rootObj["users"].toArray();

    // Find the current user and update their biodata
    for (int i = 0; i < usersArray.size(); ++i) {
        QJsonObject user = usersArray[i].toObject();
        if (user["email"].toString() == currentUserEmail) {
            QJsonObject bioData;
            bioData["age"] = age;
            bioData["weight"] = weight;
            bioData["height"] = height;
            user["bioData"] = bioData;
            usersArray[i] = user;
            break;
        }
    }

    rootObj["users"] = usersArray;
    doc.setObject(rootObj);

    // Write back to file
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
void Biodata::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Resize background
    backgroundLabel->resize(this->size());

    // Center modalFrame
    // Center modalFrame with vertical padding
    int verticalPadding = height() * 0.1;
    int modalY = verticalPadding + (height() - 2 * verticalPadding - modalFrame->height()) / 2;
    modalFrame->move((width() - modalFrame->width()) / 2, modalY);

    // Reposition silhouette
    int silhouetteX = modalFrame->x() + modalFrame->width() + 20;
    int silhouetteY = modalFrame->y() + modalFrame->height() - 200;
    silhouetteLabel->move(silhouetteX, silhouetteY);
}


void Biodata::setupBackgroundImage()
{
    // Create background label to fill entire window
    backgroundLabel = new QLabel(this);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower(); // Send to back

    // Load gym background image
    QPixmap gymBackground("images/gym.jpg");
    if (!gymBackground.isNull()) {
        // Set window minimum size to image size
        QSize imageSize = gymBackground.size();
        setMinimumSize(imageSize);  // ✅ This enforces your requirement

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


void Biodata::showBiodataPage()
{
    // Show welcome popup
    QMessageBox::information(this, "Welcome", "Welcome to FitTrack App!");

    // Show the biodata form
    this->show();
    this->raise();
    this->activateWindow();

    // Ensure modal is centered after window is shown
    int verticalPadding = height() * 0.1; // 10% padding top and bottom
    int modalY = verticalPadding + (height() - 2 * verticalPadding - modalFrame->height()) / 2;

    modalFrame->move((width() - modalFrame->width()) / 2, modalY);


    // Reposition silhouette
    int silhouetteX = modalFrame->x() + modalFrame->width() + 20;
    int silhouetteY = modalFrame->y() + modalFrame->height() - 200;
    silhouetteLabel->move(silhouetteX, silhouetteY);
}

void Biodata::handleNextStep()
{
    // Validate inputs
    QString age = ageEdit->text().trimmed();
    QString weight = weightEdit->text().trimmed();
    QString height = heightEdit->text().trimmed();

    // Basic validation
    if (age.isEmpty() || weight.isEmpty() || height.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Information",
                             "Please fill in all required fields.");
        return;
    }

    // Validate numeric inputs
    bool ageOk, weightOk, heightOk;
    int ageValue = age.toInt(&ageOk);
    double weightValue = weight.toDouble(&weightOk);
    double heightValue = height.toDouble(&heightOk);

    if (!ageOk || !weightOk || !heightOk || ageValue <= 0 || weightValue <= 0 || heightValue <= 0) {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter valid numeric values.");
        return;
    }
    //save Biodata To JSON File
    saveBiodataToFile(ageValue,weightValue,heightValue);

    /* Show success message
    QString message = QString("Biodata saved successfully!\n\n") +
                      "Age: " + age + " years\n" +
                      "Weight: " + weight + " kg\n" +
                      "Height: " + height + " cm";

    QMessageBox::information(this, "Success", message);
    */

    // Hide current form and show second biodata page
    this->hide();

    // Create and show second biodata page
    SecondBio *secondBioPage = new SecondBio(currentUserEmail);
    secondBioPage->showSecondBioPage();
}
