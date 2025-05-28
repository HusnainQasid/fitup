#include "loginwindow.h"
#include "signupwindow.h"
#include "biodata.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>
// Helper function to initialize users.json file with a test account if it doesn't exist
void initializeUserData() {
    // Ensure the data directory exists
    QDir dataDir(QDir::currentPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    QFile file(dataDir.filePath("users.json"));

    // If the file already exists, we don't need to initialize it
    if (file.exists()) {
        return;
    }

    // Create a test user
    QJsonObject rootObj;
    QJsonArray usersArray;

    QJsonObject testUser;
    testUser["email"] = "test@example.com";

    // Hash the password "password123"
    QByteArray hashedPassword = QCryptographicHash::hash(
                                    QString("password123").toUtf8(), QCryptographicHash::Sha256).toHex();
    testUser["password"] = QString(hashedPassword);
    testUser["name"] = "Test User";

    // Add more test user fields as needed
    QJsonObject bioData;
    bioData["age"] = 30;
    bioData["weight"] = 75.5;
    bioData["height"] = 180;
    bioData["gender"] = "Male";
    bioData["fitnessGoal"] = "Weight Loss";

    testUser["bioData"] = bioData;
    usersArray.append(testUser);
    rootObj["users"] = usersArray;

    QJsonDocument doc(rootObj);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Created test user: test@example.com with password: password123";
    } else {
        qDebug() << "Failed to create users.json file:" << file.errorString();
    }
}

// Helper function to ensure images directory exists with required images
void checkAndCreateImagesDir() {
    QDir imagesDir(QDir::currentPath() + "/images");
    if (!imagesDir.exists()) {
        imagesDir.mkpath(".");
        qDebug() << "Created images directory";
    }

    // Check for fitness background image
    QFile backgroundImage(imagesDir.filePath("fitness_background.jpg"));
    if (!backgroundImage.exists()) {
        qDebug() << "Warning: fitness_background.jpg not found in images directory.";
        qDebug() << "Please add a suitable image to: " << imagesDir.absolutePath();
    }

    // Check for fitness silhouette image
    QFile silhouetteImage(imagesDir.filePath("fitness_silhouette.jpg"));
    if (!silhouetteImage.exists()) {
        qDebug() << "Warning: fitness_silhouette.jpg not found in images directory.";
        qDebug() << "Please add a suitable image to: " << imagesDir.absolutePath();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application information
    QCoreApplication::setOrganizationName("FitTrack");
    QCoreApplication::setApplicationName("FitTrack Fitness App");

    // Initialize user data with a test account
    initializeUserData();

    // Check for required image files
    checkAndCreateImagesDir();

    // Create login window
    LoginWindow loginWindow;

    // Connect a custom signal for successful login
    // For example, if your LoginWindow emits a signal like 'loginSuccess()'
    // or you can do this via a lambda if you are handling login validation here.

    // Placeholder: assume your login validation is inside LoginWindow and emits a signal
    QObject::connect(&loginWindow, &LoginWindow::loginSucceeded, [&](const QString userEmail) {
        Biodata *biodataPage = new Biodata(userEmail); // Pass user email
        biodataPage->showBiodataPage();
        loginWindow.hide();
    });

    loginWindow.show();

    return app.exec();
}
