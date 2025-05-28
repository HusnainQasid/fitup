#ifndef SECONDBIO_H
#define SECONDBIO_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QCheckBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QRadioButton>
#include <QButtonGroup>

class SecondBio : public QWidget
{
    Q_OBJECT

public:
    explicit SecondBio(const QString &userEmail, QWidget *parent = nullptr);
    void showSecondBioPage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleNextStep();

private:
    void setupUI();
    void setupBackgroundImage();
    QString userEmail;
    // Main components
    QLabel *backgroundLabel;
    QFrame *modalFrame;
    QVBoxLayout *modalLayout;

    // Form elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *goalLabel;

    // Goal checkboxes
    QRadioButton *loseWeightRadio;
    QRadioButton *gainWeightRadio;
    QRadioButton *maintainWeightRadio;
    QRadioButton *buildMusclesRadio;
    QRadioButton *increaseFlexibilityRadio;
    QButtonGroup *goalButtonGroup;
    QPushButton *nextButton;

    // Silhouette image
    QLabel *silhouetteLabel;
};

#endif // SECONDBIO_H
