#ifndef FOURTHBIO_H
#define FOURTHBIO_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

class FourthBio : public QWidget
{
    Q_OBJECT

public:
    explicit FourthBio(const QString &userEmail, QWidget *parent = nullptr);
    void showFourthBioPage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleNextStep();

private:
    void setupUI();
    void setupBackgroundImage();
    bool saveDietaryRestrictions();
    QJsonDocument loadUserData();
    bool updateUserData(const QJsonObject &newData);

    // User data
    QString currentUserEmail;

    // Main components
    QLabel *backgroundLabel;
    QFrame *modalFrame;
    QVBoxLayout *modalLayout;

    // Form elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *instructionsLabel;

    // Dietary restriction checkboxes
    QCheckBox *nightshadesCheckbox;
    QCheckBox *glutenCheckbox;
    QCheckBox *highCholesterolCheckbox;
    QCheckBox *eggsCheckbox;
    QCheckBox *highSugarCheckbox;
    QCheckBox *alcoholCheckbox;
    QCheckBox *highSodiumCheckbox;
    QCheckBox *highSaturatedFatsCheckbox;
    QCheckBox *caffeinatedDrinksCheckbox;
    QCheckBox *redMeatCheckbox;

    QPushButton *nextButton;
    QLabel *silhouetteLabel;
};

#endif // FOURTHBIO_H
