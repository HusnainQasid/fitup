#include "signupwindow.h"
#include"biodata.h"
#include "loginwindow.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>
#include <QPixmap>
#include <QSpacerItem>

SignUpWindow::SignUpWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window title and size
    setWindowTitle("FitTrack - Sign Up");
    resize(800, 600);

    // Setup UI elements
    setupUI();
}

SignUpWindow::~SignUpWindow()
{
    // Qt will automatically clean up all child widgets
}

void SignUpWindow::setupUI()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);

    // Create signup panel (left side)
    signUpPanel = new QWidget();
    signUpLayout = new QVBoxLayout(signUpPanel);

    // Add top spacer
    signUpLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Title
    titleLabel = new QLabel("Sweat Now, Shine Later");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    signUpLayout->addWidget(titleLabel);

    signUpLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Name input
    nameLabel = new QLabel("Name");
    nameInput = new QLineEdit();
    nameInput->setMinimumHeight(40);
    nameInput->setPlaceholderText("Enter your name");

    signUpLayout->addWidget(nameLabel);
    signUpLayout->addWidget(nameInput);
    signUpLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Email input
    emailLabel = new QLabel("Email address");
    emailInput = new QLineEdit();
    emailInput->setMinimumHeight(40);
    emailInput->setPlaceholderText("Enter your email");

    signUpLayout->addWidget(emailLabel);
    signUpLayout->addWidget(emailInput);
    signUpLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Password input
    passwordLabel = new QLabel("Password");
    passwordInput = new QLineEdit();
    passwordInput->setMinimumHeight(40);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("password");

    signUpLayout->addWidget(passwordLabel);
    signUpLayout->addWidget(passwordInput);
    signUpLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Gender input
    genderLabel = new QLabel("Gender");
    genderComboBox = new QComboBox();
    genderComboBox->setMinimumHeight(40);
    genderComboBox->addItem("Select gender");
    genderComboBox->addItem("Male");
    genderComboBox->addItem("Female");
    genderComboBox->addItem("Non-binary");
    genderComboBox->addItem("Prefer not to say");

    signUpLayout->addWidget(genderLabel);
    signUpLayout->addWidget(genderComboBox);
    signUpLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Weight input
    weightLabel = new QLabel("Weight");
    weightInput = new QLineEdit();
    weightInput->setMinimumHeight(40);
    weightInput->setPlaceholderText("Weight");

    signUpLayout->addWidget(weightLabel);
    signUpLayout->addWidget(weightInput);
    signUpLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Terms and Conditions checkbox
    termsCheckBox = new QCheckBox("I agree to the terms & policy");
    signUpLayout->addWidget(termsCheckBox);

    signUpLayout->addItem(new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Sign up button
    signUpButton = new QPushButton("Signup");
    signUpButton->setMinimumHeight(40);
    signUpButton->setCursor(Qt::PointingHandCursor);
    signUpButton->setStyleSheet("background-color: black; color: white; border-radius: 4px; padding: 8px;");

    signUpLayout->addWidget(signUpButton);

    // Sign in link
    QHBoxLayout *signInLayout = new QHBoxLayout();

    haveAccountLabel = new QLabel("Have an account?");
    loginLink = new QPushButton("log in");
    loginLink->setFlat(true);
    loginLink->setCursor(Qt::PointingHandCursor);
    loginLink->setStyleSheet("border: none; color: blue; text-decoration: underline;");

    signInLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    signInLayout->addWidget(haveAccountLabel);
    signInLayout->addWidget(loginLink);
    signInLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    signUpLayout->addLayout(signInLayout);

    // Add bottom spacer
    signUpLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create image label (right side)
    imageLabel = new QLabel();
    imageLabel->setMinimumWidth(400);

    // Set black background with silhouette image
    QPixmap fitnessImage("images/fitness_silhouette.jpg");
    if (fitnessImage.isNull()) {
        // Fallback if image not found
        imageLabel->setStyleSheet("background-color: #000000;");
    } else {
        // Scale the image to fit the label while maintaining aspect ratio
        fitnessImage = fitnessImage.scaled(imageLabel->width(),
                                           imageLabel->height(),
                                           Qt::KeepAspectRatioByExpanding,
                                           Qt::SmoothTransformation);
        imageLabel->setPixmap(fitnessImage);
        imageLabel->setScaledContents(true);
    }

    // Add panels to main layout
    mainLayout->addWidget(signUpPanel);
    mainLayout->addWidget(imageLabel);

    // Connect slots
    connect(signUpButton, &QPushButton::clicked, this, &SignUpWindow::onSignUpButtonClicked);
    connect(loginLink, &QPushButton::clicked, this, &SignUpWindow::onLoginLinkClicked);
}

void SignUpWindow::onSignUpButtonClicked()
{
    if (!validateInputs()) {
        return;
    }

    QString name = nameInput->text().trimmed();
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    QString gender = genderComboBox->currentText();
    QString weight = weightInput->text().trimmed();

    // Check if email is already registered
    if (isEmailAlreadyRegistered(email)) {
        QMessageBox::warning(this, "Registration Failed",
                             "This email is already registered. Please use a different email or log in.");
        return;
    }

    // Save user data
    if (saveUserData(name, email, password, gender, weight)) {
        QMessageBox::information(this, "Registration Successful",
                                 "Your account has been created successfully. You can now log in.");

        // Return to login window
        Biodata *biodataPage = new Biodata(email);
        biodataPage->showBiodataPage();
        this->close();
    } else {
        QMessageBox::critical(this, "Registration Failed",
                              "There was an error saving your information. Please try again.");
    }
}

void SignUpWindow::onLoginLinkClicked()
{
    // Navigate back to login window
    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
    this->close();
}

bool SignUpWindow::validateInputs()
{
    // Check all required fields
    if (nameInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter your name.");
        nameInput->setFocus();
        return false;
    }

    QString email = emailInput->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter your email address.");
        emailInput->setFocus();
        return false;
    }

    // Basic email validation
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid email address.");
        emailInput->setFocus();
        return false;
    }

    if (passwordInput->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a password.");
        passwordInput->setFocus();
        return false;
    }

    // Simple password strength check
    if (passwordInput->text().length() < 6) {
        QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters long.");
        passwordInput->setFocus();
        return false;
    }

    if (genderComboBox->currentIndex() == 0) {
        QMessageBox::warning(this, "Validation Error", "Please select your gender.");
        genderComboBox->setFocus();
        return false;
    }

    QString weight = weightInput->text().trimmed();
    if (weight.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter your weight.");
        weightInput->setFocus();
        return false;
    }

    // Check if weight is a valid number
    bool isNumber;
    weight.toDouble(&isNumber);
    if (!isNumber) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid weight number.");
        weightInput->setFocus();
        return false;
    }

    if (!termsCheckBox->isChecked()) {
        QMessageBox::warning(this, "Validation Error", "You must agree to the terms and policy to continue.");
        return false;
    }

    return true;
}

bool SignUpWindow::saveUserData(const QString &name, const QString &email, const QString &password,
                                const QString &gender, const QString &weight)
{
    // Load existing user data
    QJsonDocument userData = loadUserData();
    QJsonObject userDataObj;

    if (userData.isNull() || !userData.isObject()) {
        // Create new user data file if it doesn't exist or is invalid
        userDataObj = QJsonObject();
        userDataObj["users"] = QJsonArray();
    } else {
        userDataObj = userData.object();
    }

    QJsonArray users = userDataObj["users"].toArray();

    // Create new user object
    QJsonObject newUser;
    newUser["name"] = name;
    newUser["email"] = email;

    // Hash the password before storing
    QByteArray hashedPassword = QCryptographicHash::hash(
                                    password.toUtf8(), QCryptographicHash::Sha256).toHex();
    newUser["password"] = QString(hashedPassword);

    // Create bioData object similar to the test user structure
    QJsonObject bioData;
    bioData["gender"] = gender;
    bioData["weight"] = weight.toDouble();
    bioData["fitnessGoal"] = "General Fitness"; // Default goal

    newUser["bioData"] = bioData;
    newUser["registrationDate"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Add user to array
    users.append(newUser);
    userDataObj["users"] = users;

    // Save updated user data
    QDir dataDir(QDir::currentPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    QFile file(dataDir.filePath("users.json"));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open users.json for writing:" << file.errorString();
        return false;
    }

    QJsonDocument updatedDoc(userDataObj);
    if (file.write(updatedDoc.toJson()) == -1) {
        qDebug() << "Failed to write to users.json:" << file.errorString();
        file.close();
        return false;
    }

    file.close();
    return true;
}

QJsonDocument SignUpWindow::loadUserData()
{
    // Ensure the data directory exists
    QDir dataDir(QDir::currentPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    QFile file(dataDir.filePath("users.json"));
    if (!file.exists()) {
        // Create an empty users file if it doesn't exist
        QJsonObject rootObj;
        rootObj["users"] = QJsonArray();

        QJsonDocument doc(rootObj);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
        } else {
            qDebug() << "Failed to create users.json file:" << file.errorString();
        }
    }

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

bool SignUpWindow::isEmailAlreadyRegistered(const QString &email)
{
    QJsonDocument userData = loadUserData();

    if (userData.isNull() || !userData.isObject()) {
        return false;
    }

    QJsonObject userDataObj = userData.object();
    QJsonArray users = userDataObj["users"].toArray();

    for (const QJsonValue &userVal : users) {
        QJsonObject user = userVal.toObject();
        if (user["email"].toString() == email) {
            return true;
        }
    }

    return false;
}
