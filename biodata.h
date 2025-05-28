#ifndef BIODATA_H
#define BIODATA_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSpacerItem>
#include <QFrame>

class Biodata : public QWidget
{
    Q_OBJECT

public:
    explicit Biodata(const QString &userEmail, QWidget *parent = nullptr);
    void showBiodataPage();

protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void handleNextStep();

private:
    QString currentUserEmail;
    void setupUI();
    void setupBackgroundImage();
    void saveBiodataToFile(int age, double weight, double height);

    // Main components
    QLabel *backgroundLabel;
    QFrame *modalFrame;
    QVBoxLayout *modalLayout;

    // Form elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *ageLabel;
    QLabel *weightLabel;
    QLabel *heightLabel;

    QLineEdit *ageEdit;
    QLineEdit *weightEdit;
    QLineEdit *heightEdit;

    QPushButton *nextButton;

    // Silhouette image
    QLabel *silhouetteLabel;
};

#endif // BIODATA_H
