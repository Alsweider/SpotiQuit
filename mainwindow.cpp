#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Icon verstecken
//    QPixmap pixmap( 32, 32 );
//    pixmap.fill( Qt::transparent );
//    setWindowIcon(QIcon(pixmap));
//    this->setWindowTitle(" ");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // Spotify.exe beenden
    ui->label->setText("Closing Spotify");
    repaint();
    Sleep(200);
    system("taskkill /f /im Spotify.exe");
    Sleep(100);

    // Spotify wieder starten
    ui->label->setText("Starting Spotify");
    repaint();
    Sleep(200);
    system("start %appdata%\\Spotify\\Spotify.exe");

    // Warten auf das Öffnen des Spotify-Fensters
    ui->label->setText("Waiting for Spotify");
     repaint();
     Sleep(200);


    HWND spotifyWindow = NULL;
    int timeout = 0;
    while (timeout < 30) {  // Maximal 30 Sekunden warten
        Sleep(1000);
        spotifyWindow = FindWindow(NULL, L"Spotify");
        if (spotifyWindow != NULL) {
            ui->label->setText("Spotify found!");
             repaint();
            Sleep(200);
            break;  // Fenster gefunden, abbrechen
        }
        timeout++;
    }

    if (spotifyWindow != NULL) {
        // Spotify-Fenster aktivieren und Leertaste senden
        ui->label->setText("Focussing Spotify");
         repaint();
        Sleep(200);
        SetForegroundWindow(spotifyWindow);
        Sleep(100);
        ui->label->setText("Starting music");
        repaint();
        Sleep(200);
        keybd_event(VK_SPACE, 0, 0, 0); // Keydown
        Sleep(100);
        keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Keyup
    } else {
        ui->label->setText("Window not found!");
         repaint();
        Sleep(200);
    }

    ui->label->setText("Ready");
     repaint();
}

