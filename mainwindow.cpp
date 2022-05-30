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

#define PI 3.14

extern "C" {
    #include "extApi.h"
}

void SimStartCheck();
void WaitPoint();
void Close();
void Open();
void Conveyor();
void PickupPoint();
void PickAndPlace1A();
void PickAndPlace1B();
void PickAndPlace2A();
void PickAndPlace2B();
void PickAndPlace3A();
void PickAndPlace3B();
void PickAndPlace4A();
void PickAndPlace4B();
void PickAndPlace5A();
void PickAndPlace5B();
void PickAndPlace6A();
void PickAndPlace6B();
void MainScript();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(550, 180));
    setWindowTitle("MainWindow");
    setWindowFlag(Qt::WindowStaysOnTopHint);

    /*
    ui->Actionneur1->setAlignment(Qt::AlignCenter);
    ui->Actionneur2->setAlignment(Qt::AlignCenter);
    ui->Actionneur3->setAlignment(Qt::AlignCenter);
    ui->Actionneur4->setAlignment(Qt::AlignCenter);
    ui->Actionneur5->setAlignment(Qt::AlignCenter);
    ui->Actionneur6->setAlignment(Qt::AlignCenter);

      afficher la position des actionneur (étape en cour mais non completé et non fonctionnel)

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

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

    simxGetObjectHandle(ClientID, "UR5_joint1", &Joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &Joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &Joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &Joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &Joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &Joint6, simx_opmode_oneshot_wait);

    simxGetJointPosition(ClientID, Joint1, &Pos1, simx_opmode_blocking);
    simxGetJointPosition(ClientID, Joint2, &Pos2, simx_opmode_blocking);
    simxGetJointPosition(ClientID, Joint3, &Pos3, simx_opmode_blocking);
    simxGetJointPosition(ClientID, Joint4, &Pos4, simx_opmode_blocking);
    simxGetJointPosition(ClientID, Joint5, &Pos5, simx_opmode_blocking);
    simxGetJointPosition(ClientID, Joint6, &Pos6, simx_opmode_blocking);

    ui->Actionneur1->setText(QString::number((double)Pos1, 'f', 1));
    ui->Actionneur2->setText(QString::number((double)Pos2, 'f', 1));
    ui->Actionneur3->setText(QString::number((double)Pos3, 'f', 1));
    ui->Actionneur4->setText(QString::number((double)Pos4, 'f', 1));
    ui->Actionneur5->setText(QString::number((double)Pos5, 'f', 1));
    ui->Actionneur6->setText(QString::number((double)Pos6, 'f', 1));

    simxFinish(ClientID);
    */
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
void MainWindow::on_Charger_clicked(){
    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);
    simxLoadScene(ClientID, "C:\\Users\\Testprod\\Desktop\\ProjetSimulateur\\ProjetSimulateur.ttt", 0, simx_opmode_blocking);
    simxFinish(ClientID);

}

void MainWindow::on_Demarrer_clicked(){
    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);
    simxStartSimulation(ClientID, simx_opmode_blocking);
    MainScript();
    simxFinish(ClientID);
}

void MainWindow::on_Arreter_clicked(){
    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);
    simxStopSimulation(ClientID, simx_opmode_blocking);
    simxFinish(ClientID);
}

        /* controle du bras et detection du proximity sensor*/
void MainScript(){

    uchar DetectionState = 0;
    float DetectedPoint;
    int DetectedObjectHandle;
    float DetectedSurfaceNormalVector;
    int ConteurBloc = 0;
    int ProxSensorSignal;

    while(ConteurBloc<6){
        simxFinish(-1);
        int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

        simxGetObjectHandle(ClientID, "/conveyor/Proximity_sensor", &ProxSensorSignal, simx_opmode_blocking);
        simxReadProximitySensor(ClientID, ProxSensorSignal, &DetectionState, &DetectedPoint, &DetectedObjectHandle, &DetectedSurfaceNormalVector, simx_opmode_blocking);

        if (DetectionState!=0){
            PickupPoint();
            switch(++ConteurBloc){
                case (1):
                    PickAndPlace1A();
                    extApi_sleepMs(1000);
                    PickAndPlace1B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace1A();
                    extApi_sleepMs(500);
                    WaitPoint();
                break;
                case (2):
                    PickAndPlace2A();
                    extApi_sleepMs(1000);
                    PickAndPlace2B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace2A();
                    extApi_sleepMs(500);
                    WaitPoint();
               break;
                case (3):
                    PickAndPlace3A();
                    extApi_sleepMs(1000);
                    PickAndPlace3B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace3A();
                    extApi_sleepMs(500);
                    WaitPoint();
                break;
                case (4):
                    PickAndPlace4A();
                    extApi_sleepMs(1000);
                    PickAndPlace4B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace4A();
                    extApi_sleepMs(500);
                    WaitPoint();
                break;
                case (5):
                    PickAndPlace5A();
                    extApi_sleepMs(1000);
                    PickAndPlace5B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace5A();
                    extApi_sleepMs(500);
                    WaitPoint();
                break;
                case (6):
                    PickAndPlace6A();
                    extApi_sleepMs(1000);
                    PickAndPlace6B();
                    Open();
                    extApi_sleepMs(500);
                    PickAndPlace6A();
                    extApi_sleepMs(500);
                    WaitPoint();
               break;
                }
            }
        simxFinish(ClientID);
        }
}

        /* ouverture du gripper */
void Open(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int motorHandle;
    float motorVelocity=0.25;
    int motorForce=30;

    simxGetObjectHandle(ClientID, "RG2_openCloseJoint#0", &motorHandle, simx_opmode_blocking);
    simxSetJointMaxForce(ClientID, motorHandle,motorForce,simx_opmode_blocking);
    simxSetJointTargetVelocity(ClientID,motorHandle,motorVelocity,simx_opmode_blocking);

    simxFinish(ClientID);
}

        /* fermeture du gripper */
void Close(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int motorHandle;
    float motorVelocity=-0.25;
    int motorForce=30;

    simxGetObjectHandle(ClientID, "RG2_openCloseJoint#0", &motorHandle, simx_opmode_blocking);
    simxSetJointMaxForce(ClientID, motorHandle,motorForce,simx_opmode_blocking);
    simxSetJointTargetVelocity(ClientID,motorHandle,motorVelocity,simx_opmode_blocking);

    extApi_sleepMs(1000);
    simxFinish(ClientID);
}

        /* retour du bras a ca position initial */
void WaitPoint(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 0.0* (PI / 180), simx_opmode_oneshot_wait);

    extApi_sleepMs(500);
    simxFinish(ClientID);
}

        /* ramassage des bloc sur convoyeur */
void PickupPoint(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 90.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 90.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, 70.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 90.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 0.0* (PI / 180), simx_opmode_oneshot_wait);

    extApi_sleepMs(1000);
    simxSetJointTargetPosition(ClientID, joint2, 20.0* (PI / 180), simx_opmode_oneshot_wait);
    extApi_sleepMs(500);
    Close();
    extApi_sleepMs(500);
    simxFinish(ClientID);
}

        /* placement des bloc au sol */
void PickAndPlace1A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 15.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 78.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -25.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 0.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace1B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 0.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 33.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 78.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -25.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 0.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace2A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 6.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 16.8* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 75.3* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -25.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 7.5* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace2B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 6.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 34.8* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 75.3* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -25.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 7.5* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace3A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 11.8* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 20.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 69.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -20.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 11.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace3B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, 11.8* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 38.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 69.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -20.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, 11.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace4A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -6.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 15.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -22.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -6.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace4B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -6.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 33.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -22.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -6.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace5A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -12.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 15.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -22.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -14.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace5B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -12.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 33.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -22.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -14.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace6A(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -18.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 15.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -24.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -20.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}

void PickAndPlace6B(){

    simxFinish(-1);
    int ClientID = simxStart("127.0.0.1", 19997, true,true,2000,5);

    int joint1= 0;
    int joint2= 0;
    int joint3= 0;
    int joint4= 0;
    int joint5= 0;
    int joint6= 0;
    double V1= 0.5;

    simxGetObjectHandle(ClientID, "UR5_joint1", &joint1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint2", &joint2, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint3", &joint3, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint4", &joint4, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint5", &joint5, simx_opmode_oneshot_wait);
    simxGetObjectHandle(ClientID, "UR5_joint6", &joint6, simx_opmode_oneshot_wait);

    simxSetJointTargetVelocity(ClientID, joint1, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint2, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint3, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint4, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint5, V1, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(ClientID, joint6, V1, simx_opmode_oneshot_wait);

    simxSetJointTargetPosition(ClientID, joint1, -18.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint2, 33.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint3, 77.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint4, -24.5* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint5, 270.0* (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetPosition(ClientID, joint6, -20.0* (PI / 180), simx_opmode_oneshot_wait);

    simxFinish(ClientID);
}




