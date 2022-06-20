#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startCommunication(QHostAddress address, int port);


private slots:

    void onChargerClicked();
    void onDemarrerClicked();
    void onArreterClicked();

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void MainScript();
    void moveToJointPosition(QVector<double> jointsPosition);
    void closeGripper();
    void openGripper();

    QPoint cur_pos;
    QPoint new_pos;

    int _clientID;
    QVector<int> _jointHandles;

    static const QVector<double> jointsPosition1A;
    static const QVector<double> jointsPosition1B;
    static const QVector<double> jointsPosition2A;
    static const QVector<double> jointsPosition2B;
    static const QVector<double> jointsPosition3A;
    static const QVector<double> jointsPosition3B;
    static const QVector<double> jointsPosition4A;
    static const QVector<double> jointsPosition4B;
    static const QVector<double> jointsPosition5A;
    static const QVector<double> jointsPosition5B;
    static const QVector<double> jointsPosition6A;
    static const QVector<double> jointsPosition6B;

    static const QVector<double> pickupPointA;
    static const QVector<double> pickupPointB;

    static const QVector<double> jointsPositionWaitPoint;


    static const QVector<QString> jointNames;

};
#endif // MAINWINDOW_H
