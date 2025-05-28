#include "thirdbio.h"
#include "secondbio.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>

SecondBio::SecondBio(const QString &userEmail, QWidget *parent) : QWidget(parent),userEmail(userEmail)
{
    setupUI();
}

void SecondBio::setupUI()
{
    // Set window properties
    setWindowTitle("FitTrack - Choose Your Goal");
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
    setMinimumSize(modalFrame->size());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
    subtitleLabel = new QLabel("Choose your Goal, What you want to achieve.");
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

    // Goal selection label
    goalLabel = new QLabel("You want to..");
    goalLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #ffffff;
            margin-bottom: 15px;
        }
    )");
    modalLayout->addWidget(goalLabel);
    // Create goal checkboxes with styling
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
        QCheckBox::indicator:checked:hover {
            background-color: #45a049;
        }
    )";

 /*   // Create checkboxes
    loseWeightCheckbox = new QCheckBox("A    Lose Weight");
    loseWeightCheckbox->setStyleSheet(checkboxStyle);

    gainWeightCheckbox = new QCheckBox("B    Gain weight");
    gainWeightCheckbox->setStyleSheet(checkboxStyle);

    maintainWeightCheckbox = new QCheckBox("C    Maintain Weight");
    maintainWeightCheckbox->setStyleSheet(checkboxStyle);

    buildMusclesCheckbox = new QCheckBox("D    Build Muscles");
    buildMusclesCheckbox->setStyleSheet(checkboxStyle);

    increaseFlexibilityCheckbox = new QCheckBox("E    Increase Flexibility");
    increaseFlexibilityCheckbox->setStyleSheet(checkboxStyle);
*/
// Remove all the QCheckBox creation code and replace with:

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
goalButtonGroup = new QButtonGroup(this);

// Create radio buttons
loseWeightRadio = new QRadioButton("A    Lose Weight");
loseWeightRadio->setStyleSheet(radioStyle);
goalButtonGroup->addButton(loseWeightRadio);

gainWeightRadio = new QRadioButton("B    Gain weight");
gainWeightRadio->setStyleSheet(radioStyle);
goalButtonGroup->addButton(gainWeightRadio);

maintainWeightRadio = new QRadioButton("C    Maintain Weight");
maintainWeightRadio->setStyleSheet(radioStyle);
goalButtonGroup->addButton(maintainWeightRadio);

buildMusclesRadio = new QRadioButton("D    Build Muscles");
buildMusclesRadio->setStyleSheet(radioStyle);
goalButtonGroup->addButton(buildMusclesRadio);

increaseFlexibilityRadio = new QRadioButton("E    Increase Flexibility");
increaseFlexibilityRadio->setStyleSheet(radioStyle);
goalButtonGroup->addButton(increaseFlexibilityRadio);

// Add to layout (replace the checkbox additions)
modalLayout->addWidget(loseWeightRadio);
modalLayout->addWidget(gainWeightRadio);
modalLayout->addWidget(maintainWeightRadio);
modalLayout->addWidget(buildMusclesRadio);
modalLayout->addWidget(increaseFlexibilityRadio);

   /* // Add checkboxes to layout
    modalLayout->addWidget(loseWeightCheckbox);
    modalLayout->addWidget(gainWeightCheckbox);
    modalLayout->addWidget(maintainWeightCheckbox);
    modalLayout->addWidget(buildMusclesCheckbox);
    modalLayout->addWidget(increaseFlexibilityCheckbox);
*/
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

    // Create silhouette placeholder matching the first page
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
    connect(nextButton, &QPushButton::clicked, this, &SecondBio::handleNextStep);
}

void SecondBio::resizeEvent(QResizeEvent *event)
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

void SecondBio::setupBackgroundImage()
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
        setMinimumSize(imageSize);

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

void SecondBio::showSecondBioPage()
{
    // Show the second biodata form
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

void SecondBio::handleNextStep()
{
    // Collect selected goals
    QStringList selectedGoals;

    if (loseWeightRadio->isChecked()) {
        selectedGoals << "Lose Weight";
    }
    if (gainWeightRadio->isChecked()) {
        selectedGoals << "Gain Weight";
    }
    if (maintainWeightRadio->isChecked()) {
        selectedGoals << "Maintain Weight";
    }
    if (buildMusclesRadio->isChecked()) {
        selectedGoals << "Build Muscles";
    }
    if (increaseFlexibilityRadio->isChecked()) {
        selectedGoals << "Increase Flexibility";
    }

    // Validate that at least one goal is selected
    if (selectedGoals.isEmpty()) {
        QMessageBox::warning(this, "No Goals Selected",
                             "Please select at least one fitness goal.");
        return;
    }
/*
    // Show success message with selected goals
    QString message = "Goals selected successfully!\n\nYour selected goals:\n";
    for (const QString &goal : selectedGoals) {
        message += "• " + goal + "\n";
    }

    QMessageBox::information(this, "Success", message);
    // TODO: Save goals data and navigate to next screen
*/
    // Create and show ThirdBio page
    ThirdBio *thirdBioPage = new ThirdBio(userEmail, selectedGoals.join(", "));
    thirdBioPage->showThirdBioPage();
    // For now, hide the form
    this->hide();
}
