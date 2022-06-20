#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QProcess"
#include <QLineEdit>
#include "simConst.h"
#include "extApi.h"
#include "extApiPlatform.h"
#include <QLabel>
#include <QTextStream>
#include <QPushButton>
#include <QGuiApplication>
#include <QtGui>

extern "C" {
    #include "extApi.h"
}

const QVector<double> MainWindow::jointsPosition1A = {0.0, 15.0, 78.0, -25.0, 270.0, 0.0};
const QVector<double> MainWindow::jointsPosition1B = {0.0, 33.0, 78.0, -25.0, 270.0, 0.0};
const QVector<double> MainWindow::jointsPosition2A = {6.0, 16.8, 75.3, -25.0, 270.0, 7.5};
const QVector<double> MainWindow::jointsPosition2B = {6.0, 34.8, 75.3, -25.0, 270.0, 7.5};
const QVector<double> MainWindow::jointsPosition3A = {11.8, 20.0, 69.0, -20.0, 270.0, 11.0};
const QVector<double> MainWindow::jointsPosition3B = {11.8, 38.0, 69.0, -20.0, 270.0, 11.0};
const QVector<double> MainWindow::jointsPosition4A = {-6.0, 15.0, 77.5, -22.5, 270.0, -6.0};
const QVector<double> MainWindow::jointsPosition4B = {-6.0, 33.0, 77.5, -22.5, 270.0, -6.0};
const QVector<double> MainWindow::jointsPosition5A = {-12.0, 15.0, 77.5, -22.5, 270.0, -14.0};
const QVector<double> MainWindow::jointsPosition5B = {-12.0, 33.0, 77.5, -22.5, 270.0, -14.0};
const QVector<double> MainWindow::jointsPosition6A = {-18.0, 15.0, 77.5, -24.5, 270.0, -20.0};
const QVector<double> MainWindow::jointsPosition6B = {-18.0, 33.0, 77.5, -24.5, 270.0, -20.0};
const QVector<double> MainWindow::pickupPointA = {90.0, 0.0, 90.0, 70.0, 90.0, 0.0};
const QVector<double> MainWindow::pickupPointB = {90.0, 20.0, 90.0, 70.0, 90.0, 0.0};
const QVector<double> MainWindow::jointsPositionWaitPoint = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};


const QVector<QString> MainWindow::jointNames = {"UR5_joint1",
                                                 "UR5_joint2",
                                                 "UR5_joint3",
                                                 "UR5_joint4",
                                                 "UR5_joint5",
                                                 "UR5_joint6"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _clientID(-1)
{
    ui->setupUi(this);
    setFixedSize(QSize(550, 180));
    setWindowTitle("MainWindow");
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QObject::connect(ui->Charger, SIGNAL(clicked()), this, SLOT(on_Charger_clicked()));
    QObject::connect(ui->Demarrer, SIGNAL(clicked()), this, SLOT(onDemarrerClicked()));
    QObject::connect(ui->Arreter, SIGNAL(clicked()), this, SLOT(onArreterClicked()));

    /*
    ui->Actionneur1->setAlignment(Qt::AlignCenter);
    ui->Actionneur2->setAlignment(Qt::AlignCenter);
    ui->Actionneur3->setAlignment(Qt::AlignCenter);
    ui->Actionneur4->setAlignment(Qt::AlignCenter);
    ui->Actionneur5->setAlignment(Qt::AlignCenter);
    ui->Actionneur6->setAlignment(Qt::AlignCenter);

      afficher la position des actionneur (étape en cour mais non completé et non fonctionnel)

    simxFinish(-1);
    int _clientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int Joint1= 0;
    int Joint2= 0;
    int Joint3= 0;
    int Joint4= 0;
    int Joint5= 0;
    int Joint6= 0;
    float Pos1= 0.0;
    float Pos2= 0.0;
    float Pos3= 0.0;
    float Pos4= 0.0;
    float Pos5= 0.0;
    float Pos6= 0.0;

    simxGetObjectHandle(_clientID, "UR5_joint1", &Joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(_clientID, "UR5_joint2", &Joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(_clientID, "UR5_joint3", &Joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(_clientID, "UR5_joint4", &Joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(_clientID, "UR5_joint5", &Joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(_clientID, "UR5_joint6", &Joint6, simx_opmode_oneshot_wait);

    simxGetJointPosition(_clientID, Joint1, &Pos1, simx_opmode_blocking);
    simxGetJointPosition(_clientID, Joint2, &Pos2, simx_opmode_blocking);
    simxGetJointPosition(_clientID, Joint3, &Pos3, simx_opmode_blocking);
    simxGetJointPosition(_clientID, Joint4, &Pos4, simx_opmode_blocking);
    simxGetJointPosition(_clientID, Joint5, &Pos5, simx_opmode_blocking);
    simxGetJointPosition(_clientID, Joint6, &Pos6, simx_opmode_blocking);

    ui->Actionneur1->setText(QString::number((double)Pos1, 'f', 1));
    ui->Actionneur2->setText(QString::number((double)Pos2, 'f', 1));
    ui->Actionneur3->setText(QString::number((double)Pos3, 'f', 1));
    ui->Actionneur4->setText(QString::number((double)Pos4, 'f', 1));
    ui->Actionneur5->setText(QString::number((double)Pos5, 'f', 1));
    ui->Actionneur6->setText(QString::number((double)Pos6, 'f', 1));

    simxFinish(_clientID);
    */
}

void MainWindow::startCommunication(QHostAddress address, int port)
{
    _clientID = simxStart(address.toString().toStdString().c_str(), port, true, true, 2000, 5);
}

MainWindow::~MainWindow(){
    delete ui;
}
            /* permet de controler la fenêtre avec la souris de nimporte quel endroit dans la fenêtre*/
void MainWindow::mousePressEvent(QMouseEvent *event){
        cur_pos = event->globalPosition().toPoint();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
        new_pos = QPoint(event->globalPosition().toPoint() - cur_pos);
        move(x() + new_pos.x(), y() + new_pos.y());
        cur_pos = event->globalPosition().toPoint();
}
               /* charger, démarrer et arrêter la simulation j'avais prevue faire une recherche avec Qdir pour
                  chercher automatiquement le path du .exe*/
void MainWindow::onChargerClicked(){

    Q_ASSERT(_clientID != -1);
    //simxLoadScene(_clientID, "C:\\Users\\Testprod\\Desktop\\ProjetSimulateur\\ProjetSimulateur.ttt", 0, simx_opmode_blocking);
    int result = simxLoadScene(_clientID, "/home/gelacmalac/git/Projet-Simulateur-/ProjetSimulateur.ttt", 0, simx_opmode_blocking);

    if(0 == result)
    {
        for(int i=0; (i<6) && (0 == result) ; ++i)
        {
            result = simxGetObjectHandle(_clientID, jointNames[0].toLatin1(), &_jointHandles[0], simx_opmode_oneshot_wait);
            if(result != 0)
            {
                simxFinish(-1);
                _clientID = -1;
            }
        }
    }
}

void MainWindow::onDemarrerClicked(){
    simxFinish(-1);
    int _clientID = simxStart("127.0.0.1", 19997, true,true,2000,5);
    simxStartSimulation(_clientID, simx_opmode_blocking);
    MainScript();
    simxFinish(_clientID);
}

void MainWindow::onArreterClicked(){
    simxFinish(-1);
    int _clientID = simxStart("127.0.0.1", 19997, true,true,2000,5);
    simxStopSimulation(_clientID, simx_opmode_blocking);
    simxFinish(_clientID);
}

// controle du bras et detection du proximity sensor
void MainWindow::MainScript()
{
    Q_ASSERT(_clientID != -1);

    uchar DetectionState = 0;
    float DetectedPoint;
    int DetectedObjectHandle;
    float DetectedSurfaceNormalVector;
    int ConteurBloc = 0;
    int ProxSensorSignal;

    while(ConteurBloc<6){

        simxGetObjectHandle(_clientID, "/conveyor/Proximity_sensor", &ProxSensorSignal, simx_opmode_blocking);
        simxReadProximitySensor(_clientID, ProxSensorSignal, &DetectionState, &DetectedPoint, &DetectedObjectHandle, &DetectedSurfaceNormalVector, simx_opmode_blocking);

        if (DetectionState!=0){
            moveToJointPosition(pickupPointA);
            extApi_sleepMs(1000);
            moveToJointPosition(pickupPointB);
            extApi_sleepMs(500);
            closeGripper();
            extApi_sleepMs(500);

            switch(++ConteurBloc){
                case (1):
                    moveToJointPosition(jointsPosition1A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition1B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition1A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
                break;
                case (2):
                    moveToJointPosition(jointsPosition2A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition2B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition2A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
               break;
                case (3):
                    moveToJointPosition(jointsPosition3A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition3B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition3A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
                break;
                case (4):
                    moveToJointPosition(jointsPosition4A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition4B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition4A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
                break;
                case (5):
                    moveToJointPosition(jointsPosition5A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition5B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition5A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
                break;
                case (6):
                    moveToJointPosition(jointsPosition6A);
                    extApi_sleepMs(1000);
                    moveToJointPosition(jointsPosition6B);
                    openGripper();
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPosition6A);
                    extApi_sleepMs(500);
                    moveToJointPosition(jointsPositionWaitPoint);
                    extApi_sleepMs(500);
               break;
                }
            }
        }
}

// ouverture du gripper
void MainWindow::openGripper()
{
    Q_ASSERT(_clientID != -1);

    int motorHandle;
    float motorVelocity=0.25;
    int motorForce=30;

    simxGetObjectHandle(_clientID, "RG2_openCloseJoint#0", &motorHandle, simx_opmode_blocking);
    simxSetJointMaxForce(_clientID, motorHandle,motorForce,simx_opmode_blocking);
    simxSetJointTargetVelocity(_clientID,motorHandle,motorVelocity,simx_opmode_blocking);
}

// fermeture du gripper
void MainWindow::closeGripper()
{
    Q_ASSERT(_clientID != -1);

    int motorHandle;
    float motorVelocity=-0.25;
    int motorForce=30;

    simxGetObjectHandle(_clientID, "RG2_openCloseJoint#0", &motorHandle, simx_opmode_blocking);
    simxSetJointMaxForce(_clientID, motorHandle,motorForce,simx_opmode_blocking);
    simxSetJointTargetVelocity(_clientID,motorHandle,motorVelocity,simx_opmode_blocking);

    extApi_sleepMs(1000);
}

// placement des bloc au sol
void MainWindow::moveToJointPosition(QVector<double> jointsPosition)
{
    Q_ASSERT(_clientID != -1);

    double V1= 0.5;

    for(int i=0; i<6; ++i)
    {
        simxSetJointTargetVelocity(_clientID, _jointHandles[0], V1, simx_opmode_oneshot_wait);
        simxSetJointTargetPosition(_clientID, _jointHandles[0], qDegreesToRadians(jointsPosition[i]), simx_opmode_oneshot_wait);
    }
}






