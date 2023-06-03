#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWebEngineView>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void updateFieldSizes();
    void setInputFieldsVisibility();
    void setMapLocation();

private:
    QWebEngineView *mapView;
    QLineEdit *startAddressInput;
    QLineEdit *endAddressInput;
    QLineEdit *startLatInput;
    QLineEdit *startLngInput;
    QLineEdit *endLatInput;
    QLineEdit *endLngInput;
    QLineEdit *maxDistanceInput;
    QComboBox *inputType;

    QLabel *startAddressLabel;
    QLabel *endAddressLabel;
    QLabel *startLatLabel;
    QLabel *startLngLabel;
    QLabel *endLatLabel;
    QLabel *endLngLabel;
    QLabel *maxDistanceLabel;
};

#endif // MAINWINDOW_H
