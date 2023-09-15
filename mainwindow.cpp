#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // Spotify.exe beenden
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Closing Spotify";
    ui->label->setText("Closing Spotify");
    repaint();
    Sleep(200);
    system("taskkill /f /im Spotify.exe");
    Sleep(100);

    // Spotify wieder starten
    ui->label->setText("Starting Spotify");
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Starting Spotify";
    repaint();
    Sleep(200);
    //Spotify starten
    system("start %appdata%\\Spotify\\Spotify.exe");

    // Warten auf das Öffnen des Spotify-Fensters
    ui->label->setText("Waiting for Spotify");
    repaint();
     Sleep(100);

    HWND spotifyWindow = NULL;
    int timeout = 0;
    qDebug() <<  QTime::currentTime().toString("hh:mm:ss:zzz") + " - Waiting for Spotify";

    while (timeout < 200) {
        Sleep(100);
        spotifyWindow = FindWindow(NULL, L"Spotify");
        if (spotifyWindow != NULL) {
            ui->label->setText("Spotify found!");
            qDebug() <<  QTime::currentTime().toString("hh:mm:ss:zzz") + " - Spotify found";
            repaint();
            Sleep(100);
            break;  // Fenster gefunden, abbrechen
        }
        timeout++;
    }

    if (spotifyWindow != NULL) {
        // Spotify-Fenster aktivieren und Leertaste senden
        ui->label->setText("Focussing on Spotify");
         repaint();
        Sleep(200);
        SetForegroundWindow(spotifyWindow);
        ui->label->setText("Spotify focussed");
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + "- Spotify set as foreground window";

        Sleep(100);
        ui->label->setText("Starting music");
        repaint();
        Sleep(100);

         if (GetForegroundWindow() == spotifyWindow){
            ui->label->setText("Spotify is active window");
            repaint();
            if (ui->radioButton->isChecked()){
                //Medientaste play/pause
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0); // Keydown
                // Sleep(20);
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Media play/pause pressed";
            } else if (ui->radioButton_2->isChecked()){
                //Leertaste
                keybd_event(VK_SPACE, 0, 0, 0); // Keydown
                Sleep(100);
                keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") +  " - Spacebar pressed";
            }

            //Spotify minimieren
            if (ui->checkBox->isChecked()){
                ShowWindow(spotifyWindow, SW_MINIMIZE);
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Spotify minimised";

            }


        }


    } else {
        ui->label->setText("Window not found!");
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Window not found!";
        repaint();
        Sleep(200);
    }

    ui->label->setText("Ready");
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Finished";
    repaint();
}

