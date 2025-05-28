#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H
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
#include <QComboBox>
#include <QCryptographicHash>

class LoginWindow;

class SignUpWindow : public QMainWindow
{
    Q_OBJECT

public:
    SignUpWindow(QWidget *parent = nullptr);
    ~SignUpWindow();

private slots:
    void onSignUpButtonClicked();
    void onLoginLinkClicked();

private:
    // UI Elements
    QLabel *titleLabel;
    QLabel *nameLabel;
    QLineEdit *nameInput;
    QLabel *emailLabel;
    QLineEdit *emailInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QLabel *genderLabel;
    QComboBox *genderComboBox;
    QLabel *weightLabel;
    QLineEdit *weightInput;
    QCheckBox *termsCheckBox;
    QPushButton *signUpButton;
    QLabel *haveAccountLabel;
    QPushButton *loginLink;
    QLabel *imageLabel;

    // Layout Management
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QWidget *signUpPanel;
    QVBoxLayout *signUpLayout;

    // Helper methods
    void setupUI();
    bool validateInputs();
    bool saveUserData(const QString &name, const QString &email, const QString &password,
                      const QString &gender, const QString &weight);
    QJsonDocument loadUserData();
    bool isEmailAlreadyRegistered(const QString &email);
};

#endif // SIGNUPWIND
