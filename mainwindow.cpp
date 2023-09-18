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
    //Variable für die Pausenzeit erhält ihren Wert von der Spinbox.
     int pauseTime = ui->spinBox->value();
    // Spotify.exe beenden
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Closing Spotify";
    ui->label->setText("Closing Spotify");
    repaint();
    Sleep(pauseTime);
    system("taskkill /f /im Spotify.exe");
    Sleep(pauseTime);

    // Spotify wieder starten
    ui->label->setText("Starting Spotify");
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Starting Spotify";
    repaint();
    Sleep(pauseTime);
    //Spotify starten
    system("start %appdata%\\Spotify\\Spotify.exe");

    // Warten auf das Öffnen des Spotify-Fensters
    ui->label->setText("Waiting for Spotify");
    repaint();
     Sleep(pauseTime);

    HWND spotifyWindow = NULL;
    int timeout = 0;
    qDebug() <<  QTime::currentTime().toString("hh:mm:ss:zzz") + " - Waiting for Spotify";

    while (timeout < 400) {
        Sleep(50);
        spotifyWindow = FindWindow(NULL, L"Spotify");
        if (spotifyWindow != NULL) {
            ui->label->setText("Spotify found!");
            qDebug() <<  QTime::currentTime().toString("hh:mm:ss:zzz") + " - Spotify found";
            repaint();
            Sleep(pauseTime);
            break;  // Fenster gefunden, abbrechen
        }
        timeout++;
    }

    //Ist Checkbox zum automatischen Fortsetzen aktiv?
    if (ui->resumePlaybackCheckBox->isChecked()) {

    if (spotifyWindow != NULL) {
        // Spotify-Fenster aktivieren und Leertaste senden
        ui->label->setText("Focussing on Spotify");
         repaint();
        Sleep(pauseTime);
        SetForegroundWindow(spotifyWindow);
        ui->label->setText("Spotify focussed");
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + "- Spotify set as foreground window";

        Sleep(pauseTime);
        ui->label->setText("Starting music");
        repaint();
        Sleep(pauseTime);

         if (GetForegroundWindow() == spotifyWindow){
            ui->label->setText("Spotify is active window");
            repaint();
            if (ui->radioButton->isChecked()){
                //Medientaste play/pause
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0); // Keydown
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Media play/pause pressed";
            } else if (ui->radioButton_2->isChecked()){
                //Leertaste
                keybd_event(VK_SPACE, 0, 0, 0); // Keydown
                keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") +  " - Spacebar pressed";
            }


        }




    } else {
        ui->label->setText("Window not found!");
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Window not found!";
        repaint();
        Sleep(pauseTime);
    }

    } //Neustartprozess und Fortsetzen Ende

    //Spotify minimieren
    if (ui->checkBox->isChecked()){
    ShowWindow(spotifyWindow, SW_MINIMIZE);
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Spotify minimised";

    }

    ui->label->setText("Ready");
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Finished";
    repaint();
}


void MainWindow::on_pushButton_3_clicked()
{
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0); // Keyup
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Next track.";
}


void MainWindow::on_pushButton_2_clicked()
{
    keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0); // Keyup
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Previous track";
}




void MainWindow::on_resumePlaybackCheckBox_clicked()
{
    if (!ui->resumePlaybackCheckBox->isChecked()) {
    // Die Checkbox wurde abgewählt, Radio-Buttons deaktivieren
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    } else {
    //Checkbox aktiv, Radio-Buttons aktivieren
    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    }


}


void MainWindow::on_playPauseButton_clicked()
{
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0); // Keyup
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Media play/pause pressed";
}

