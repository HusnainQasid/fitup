#include "loginwindow.h"
#include "signupwindow.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>
#include <QPixmap>
#include <QSpacerItem>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), signUpWindow(nullptr)  // Initialize signUpWindow to nullptr
{
    // Set window title and size
    setWindowTitle("FitTrack - Login");
    resize(800, 600);

    // Setup UI elements
    setupUI();

    // Check for saved session
    loadSavedSession();
}

LoginWindow::~LoginWindow()
{
    // Clean up the signup window if it exists
    if (signUpWindow) {
        delete signUpWindow;
    }
}

void LoginWindow::setupUI()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);

    // Create login panel (left side)
    loginPanel = new QWidget();
    loginLayout = new QVBoxLayout(loginPanel);

    // Add top spacer
    loginLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Welcome labels
    welcomeLabel = new QLabel("Welcome back!");
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(20);
    welcomeFont.setBold(true);
    welcomeLabel->setFont(welcomeFont);

    subtitleLabel = new QLabel("Enter your Credentials to access your account");

    loginLayout->addWidget(welcomeLabel);
    loginLayout->addWidget(subtitleLabel);
    loginLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Email input
    emailLabel = new QLabel("Email address");
    emailInput = new QLineEdit();
    emailInput->setMinimumHeight(40);
    emailInput->setPlaceholderText("Enter your email");

    loginLayout->addWidget(emailLabel);
    loginLayout->addWidget(emailInput);
    loginLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Password input with forgot password link
    QHBoxLayout *passwordHeaderLayout = new QHBoxLayout();
    passwordLabel = new QLabel("Password");
    forgotPasswordLink = new QPushButton("forgot password");
    forgotPasswordLink->setFlat(true);
    forgotPasswordLink->setCursor(Qt::PointingHandCursor);
    forgotPasswordLink->setStyleSheet("border: none; color: blue; text-decoration: underline;");

    passwordHeaderLayout->addWidget(passwordLabel);
    passwordHeaderLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    passwordHeaderLayout->addWidget(forgotPasswordLink);

    loginLayout->addLayout(passwordHeaderLayout);

    passwordInput = new QLineEdit();
    passwordInput->setMinimumHeight(40);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Enter your password");

    loginLayout->addWidget(passwordInput);

    // Remember me checkbox
    rememberMeCheckBox = new QCheckBox("Remember for 30 days");
    loginLayout->addWidget(rememberMeCheckBox);

    loginLayout->addItem(new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Login button
    loginButton = new QPushButton("Login");
    loginButton->setMinimumHeight(40);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet("background-color: black; color: white; border-radius: 4px; padding: 8px;");

    loginLayout->addWidget(loginButton);

    loginLayout->addItem(new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Sign up button (replacement for social sign-in buttons)
    signUpButton = new QPushButton("Create New Account");
    signUpButton->setMinimumHeight(40);
    signUpButton->setCursor(Qt::PointingHandCursor);
    signUpButton->setStyleSheet("background-color: #f0f0f0; color: black; border: 1px solid #ccc; border-radius: 4px; padding: 8px;");

    loginLayout->addWidget(signUpButton);

    // Sign up link
    QHBoxLayout *signUpLayout = new QHBoxLayout();

    noAccountLabel = new QLabel("Don't have an account?");
    signUpLink = new QPushButton("Sign Up");
    signUpLink->setFlat(true);
    signUpLink->setCursor(Qt::PointingHandCursor);
    signUpLink->setStyleSheet("border: none; color: blue; text-decoration: underline;");

    // Add stretch to push the label and link to the center
    signUpLayout->addStretch();
    signUpLayout->addWidget(noAccountLabel);
    signUpLayout->addWidget(signUpLink);
    signUpLayout->addStretch();

    loginLayout->addLayout(signUpLayout);

    // Add bottom spacer
    loginLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create image label (right side)
    imageLabel = new QLabel();
    imageLabel->setMinimumWidth(400);

    // Load and display fitness image
    QPixmap fitnessImage("images/fitness_background.jpg.png");
    if (fitnessImage.isNull()) {
        // Fallback if image not found in resources
        imageLabel->setStyleSheet("background-color: #3498db; color: white;");
        imageLabel->setText("FitTrack\nYour Fitness Journey\nStarts Here");
        imageLabel->setAlignment(Qt::AlignCenter);
        QFont imgFont = imageLabel->font();
        imgFont.setPointSize(18);
        imgFont.setBold(true);
        imageLabel->setFont(imgFont);
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
    mainLayout->addWidget(loginPanel);
    mainLayout->addWidget(imageLabel);

    // Connect slots
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(signUpLink, &QPushButton::clicked, this, &LoginWindow::onSignUpLinkClicked);
    connect(forgotPasswordLink, &QPushButton::clicked, this, &LoginWindow::onForgotPasswordLinkClicked);
    connect(signUpButton, &QPushButton::clicked, this, &LoginWindow::onSignUpButtonClicked);
}

void LoginWindow::onLoginButtonClicked()
{
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    bool rememberMe = rememberMeCheckBox->isChecked();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both email and password.");
        return;
    }

    if (validateCredentials(email, password)) {
        // Save session if remember me is checked
        if (rememberMe) {
            saveUserSession(email, true);
        } else {
            // Clear any saved sessions
            QSettings settings("FitTrack", "UserSession");
            settings.remove("savedEmail");
            settings.remove("rememberMe");
        }

        // Store current user email for use in other parts of the app
        currentUserEmail = email;

        // Show success message
        QMessageBox::information(this, "Login Successful", "Welcome back to FitTrack!");

        // Emit the login succeeded signal
        emit loginSucceeded(email);

        // Hide the login window (ready for main app navigation)
        this->hide();

        // TODO: Navigate to main application window/dashboard here
        // Example: MainWindow *mainWindow = new MainWindow(email);
        //          mainWindow->show();

    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid email or password.");
    }
}

void LoginWindow::onSignUpLinkClicked()
{
    // Create the signup window if it doesn't exist
    if (!signUpWindow) {
        signUpWindow = new SignUpWindow(this);  // 'this' sets the parent
    }

    // Hide the login window and show the signup window
    this->hide();
    signUpWindow->show();
    signUpWindow->raise();
    signUpWindow->activateWindow();
}

void LoginWindow::onForgotPasswordLinkClicked()
{
    QMessageBox::information(this, "Password Recovery",
                             "Password recovery functionality will be implemented in the future.");
}

void LoginWindow::onSignUpButtonClicked()
{
    // This method replaces the social sign-in buttons functionality
    // Simply redirect to the sign-up page
    onSignUpLinkClicked();
}

bool LoginWindow::validateCredentials(const QString &email, const QString &password)
{
    QJsonDocument userData = loadUserData();

    if (userData.isNull() || !userData.isObject()) {
        qDebug() << "User data file is empty or not correctly formatted";
        return false;
    }

    QJsonObject userDataObj = userData.object();
    QJsonArray users = userDataObj["users"].toArray();

    // Hash the input password for comparison
    QByteArray hashedPassword = QCryptographicHash::hash(
                                    password.toUtf8(), QCryptographicHash::Sha256).toHex();

    for (const QJsonValue &userVal : users) {
        QJsonObject user = userVal.toObject();
        if (user["email"].toString() == email &&
            user["password"].toString() == QString(hashedPassword)) {
            return true;
        }
    }

    return false;
}

QJsonDocument LoginWindow::loadUserData()
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

void LoginWindow::saveUserSession(const QString &email, bool rememberMe)
{
    QSettings settings("FitTrack", "UserSession");
    settings.setValue("savedEmail", email);
    settings.setValue("rememberMe", rememberMe);
}

void LoginWindow::loadSavedSession()
{
    QSettings settings("FitTrack", "UserSession");
    bool rememberMe = settings.value("rememberMe", false).toBool();

    if (rememberMe) {
        QString savedEmail = settings.value("savedEmail").toString();
        if (!savedEmail.isEmpty()) {
            emailInput->setText(savedEmail);
            rememberMeCheckBox->setChecked(true);
            // Focus on password field
            passwordInput->setFocus();
        }
    }
}
