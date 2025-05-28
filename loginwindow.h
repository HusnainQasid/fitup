#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

// Forward declaration of SignUpWindow to avoid circular includes
class SignUpWindow;

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSucceeded(const QString &userEmail);  // Add this signal

private slots:
    void onLoginButtonClicked();
    void onSignUpLinkClicked();
    void onForgotPasswordLinkClicked();
    void onSignUpButtonClicked();

private:
    // UI Elements
    QLabel *welcomeLabel;
    QLabel *subtitleLabel;
    QLabel *emailLabel;
    QLineEdit *emailInput;
    QLabel *passwordLabel;
    QPushButton *forgotPasswordLink;
    QLineEdit *passwordInput;
    QCheckBox *rememberMeCheckBox;
    QPushButton *loginButton;
    QPushButton *signUpButton;
    QLabel *noAccountLabel;
    QPushButton *signUpLink;
    QLabel *imageLabel;

    // Layout Management
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QWidget *loginPanel;
    QVBoxLayout *loginLayout;

    // Window Management
    SignUpWindow *signUpWindow;  // Pointer to signup window

    // Helper methods
    void setupUI();
    bool validateCredentials(const QString &email, const QString &password);
    QJsonDocument loadUserData();
    void saveUserSession(const QString &email, bool rememberMe);
    void loadSavedSession();
    QString currentUserEmail;
};

#endif // LOGINWINDOW_H
