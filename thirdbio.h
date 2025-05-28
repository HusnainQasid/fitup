#ifndef THIRDBIO_H
#define THIRDBIO_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QSpacerItem>
#include <QRadioButton>
#include <QButtonGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

class ThirdBio : public QWidget
{
    Q_OBJECT

public:
    explicit ThirdBio(const QString &userEmail, const QString &selectedGoal, QWidget *parent = nullptr);
    void showThirdBioPage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleNextStep();

private:
    void setupUI();
    void setupBackgroundImage();
    bool saveTimeFrameData();
    QJsonDocument loadUserData();
    bool updateUserData(const QJsonObject &newData);

    // User data
    QString currentUserEmail;
    QString fitnessGoal;

    // Main components
    QLabel *backgroundLabel;
    QFrame *modalFrame;
    QVBoxLayout *modalLayout;

    // Form elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *noteLabel;
    QLabel *noteDescLabel;

    // Time frame radio buttons
    QRadioButton *twoMonthsRadio;
    QRadioButton *fourMonthsRadio;
    QRadioButton *sixMonthsRadio;
    QRadioButton *moreThanSixRadio;
    QButtonGroup *timeFrameButtonGroup;

    QPushButton *nextButton;

    // Silhouette image
    QLabel *silhouetteLabel;
};

#endif // THIRDBIO_H
