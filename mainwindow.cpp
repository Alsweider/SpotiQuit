#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setze Standardpfad zu Spotify.exe in Textfeld
    pfadSetzen();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //Variable für die Pausenzeit erhält ihren Wert von der spinBox.
     int pauseTime = ui->spinBox->value();
    //Spotify-Pfad einlesen
    QString spotifyExePath = ui->lineEdit->text();

     //Wenn Spotify geöffnet ist, Spotify schließen
     if (IsSpotifyOpen()) {
        // Spotify.exe beenden
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Closing Spotify";
        ui->label->setText("Closing Spotify");
        Sleep(pauseTime);
        system("taskkill /f /im Spotify.exe");
        Sleep(pauseTime);
     }

     //Wenn Spotify geschlossen ist, Spotify öffnen
     if (!IsSpotifyOpen()){
        qDebug() << "Spotify is not started";
        qDebug() << "Trying to start Spotify";
        ui->label->setText("Starting Spotify");
        std::string spotifyExePathStdString = spotifyExePath.toStdString();
        std::string startCommand = "start " + spotifyExePathStdString;
        system(startCommand.c_str()); // startCommand als Befehl
     }

     //Nun mal schauen, ob wir das Fenster erhaschen können...

     for (int startversuch = 1; startversuch <=400; startversuch++){
        HWND spotifyWindow = FindWindow(NULL, L"Spotify");
        SetForegroundWindow(spotifyWindow);

        if (GetForegroundWindow() == spotifyWindow){
            qDebug() << "Spotify in den Vordergrund geholt.";
            ui->label->setText("Spotify active");

            if (ui->resumePlaybackCheckBox->isChecked()){

            if (ui->radioButton->isChecked()){
                //Medientaste play/pause
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0); // Keydown
                keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Media play/pause pressed";
                ui->label->setText("Play/Pause pressed");
            } else if (ui->radioButton_2->isChecked()){
                //Leertaste
                keybd_event(VK_SPACE, 0, 0, 0); // Keydown
                keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Keyup
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") +  " - Spacebar pressed";
                ui->label->setText("Spacebar pressed");
            }

            }

            Sleep(pauseTime);

            //Wenn die Herrschaften es wünschen, Spotify minimieren
            if (ui->checkBox->isChecked()){
            ShowWindow(spotifyWindow, SW_SHOWMINIMIZED); //aktivieren, minimieren
            qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Spotify minimised";
            ui->label->setText("Spotify minimised");
            }

            ui->label->setText("Ready");

            break;

        } else {
            ui->label->setText("Getting Spotify...");
            repaint();
            qDebug() << "Spotify-Fenster nicht im Vordergrund. " << startversuch;
            Sleep(50);
            if (startversuch == 400){
            ui->label->setText("Spotify not found");
            }
        }
     }
}

//Die Medientasten
//Vorwärts-Taste
void MainWindow::on_pushButton_3_clicked()
{
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0); // Keyup
}

//Rückwärts-Taste
void MainWindow::on_pushButton_2_clicked()
{
    keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0); // Keyup
}

//Play-Pause-Taste
void MainWindow::on_playPauseButton_clicked()
{
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0); // Keydown
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0); // Keyup
}

//Knöpfe zur Auswahl der Abspieltaste aktivieren / deaktivieren
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


//Pfadauswahl
void MainWindow::on_pushButton_4_clicked()
{
    // Benutzer nach dem Pfad zur Spotify.exe-Datei fragen
    QString spotifyExePath = QFileDialog::getOpenFileName(this, "Spotify.exe auswählen", QDir::homePath(), "Ausführbare Dateien (*.exe)");

        if (!spotifyExePath.isEmpty()) {

        //Schrägstriche umdrehen
         spotifyExePath.replace("/", "\\");

         // Pfad bei Bedarf in 8.3-Kurznamen umwandeln
         WCHAR shortPath[MAX_PATH];
         if (GetShortPathName((LPCWSTR)spotifyExePath.utf16(), shortPath, MAX_PATH)) {
            spotifyExePath = QString::fromUtf16(reinterpret_cast<const char16_t*>(shortPath));
         }

        // Den Pfad in das lineEdit-Widget setzen
         ui->lineEdit->setText(spotifyExePath);
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Path set: " << spotifyExePath;
        } else {
            qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Path error";
        }
}


//Prüfung ob die Taskliste Spotify.exe enthält
bool MainWindow::IsSpotifyOpen() {
        QProcess process;
        process.start("tasklist");
        process.waitForFinished();
        QString taskListOutput = process.readAllStandardOutput();
        return taskListOutput.contains("Spotify.exe");
}


//Setze Standardpfad zu Spotify.exe in Textfeld
void MainWindow::pfadSetzen(){
        // Den Pfad zur Spotify.exe setzen
        QString appDataPath = QProcessEnvironment::systemEnvironment().value("APPDATA");
        QString localAppDataPath = QProcessEnvironment::systemEnvironment().value("LOCALAPPDATA");
        QString spotifyExePath = appDataPath + "\\Spotify\\Spotify.exe";

        // Überprüfen, ob Spotify.exe im LocalAppData-Verzeichnis vorhanden ist (falls nicht, %appdata% verwenden)
        if (!QFile::exists(spotifyExePath)) {
            spotifyExePath = localAppDataPath + "\\Spotify\\Spotify.exe";
        }

        // Pfad bei Bedarf in 8.3-Kurznamen umwandeln
        WCHAR shortPath[MAX_PATH];
        if (GetShortPathName((LPCWSTR)spotifyExePath.utf16(), shortPath, MAX_PATH)) {
            spotifyExePath = QString::fromUtf16(reinterpret_cast<const char16_t*>(shortPath));
        }

        // Den Pfad in das lineEdit-Widget setzen
        ui->lineEdit->setText(spotifyExePath);
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Path set: " << spotifyExePath;
}


//Pfad zurücksetzen
void MainWindow::on_pushButton_5_clicked()
{
        pfadSetzen();
}

