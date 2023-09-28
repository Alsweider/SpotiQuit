#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QFileDialog>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setze Standardpfad zu Spotify.exe in Textfeld
    pfadSetzen();

    //Gespeicherte Einstellungen wiederherstellen
    loadSettings();

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

        if (ui->checkBoxGentleClosing->isChecked()){
                system("taskkill /im Spotify.exe");

            ui->label->setText("Waiting for quit...");
            repaint();

            //Erfolgsprüfung von taskkill
            while (IsSpotifyOpen()){
                Sleep(20);
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Waiting for Spotify to quit gently";
                }

        } else {
            system("taskkill /t /f /im Spotify.exe");

                ui->label->setText("Waiting for quit...");
                repaint();

            //Erfolgsprüfung von taskkill
            while (IsSpotifyOpen()){
                Sleep(20);
                qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Waiting for Spotify to quit forcefully";
            }
        }

     }

     //Wenn Spotify geschlossen ist, Spotify öffnen
     if (!IsSpotifyOpen()){
        qDebug() << "Spotify is not started";
        qDebug() << "Trying to start Spotify";
        ui->label->setText("Starting Spotify");
        std::string spotifyExePathStdString = spotifyExePath.toStdString();
        std::string startCommand = "start " + spotifyExePathStdString;
        system(startCommand.c_str()); // startCommand als Befehl

        ui->label->setText("Waiting for start...");
        repaint();

        //Erfolgskontrolle Öffnen
        while (!IsSpotifyOpen()){
            Sleep(20);
            qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Waiting for Spotify to start";
        }

     }

    //Pause verhindert vorzeitige Tastenaktion
    // Sleep(pauseTime);

     //Nun mal schauen, ob wir das Fenster erhaschen können...

     for (int startversuch = 1; startversuch <=400; startversuch++){
        HWND spotifyWindow = FindWindow(NULL, L"Spotify");
        SetForegroundWindow(spotifyWindow);

        if (GetForegroundWindow() == spotifyWindow){
            qDebug() << "Spotify in den Vordergrund geholt.";
            ui->label->setText("Spotify active");

            //Pause verhindert vorzeitige Tastenaktion
            Sleep(pauseTime);

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
            if (startversuch == 400 && GetForegroundWindow() != spotifyWindow){
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

//Tastenkürzel bauen
void MainWindow::showEvent(QShowEvent* event)
{
        QMainWindow::showEvent(event);

        int modifier1 = MOD_CONTROL;
        int modifier2 = MOD_ALT;
        int hotkey = 'S';

        //Kürzel für den Spotify-Neustart global in Windows registrieren: Strg+Alt+S
        //RegisterHotKey(reinterpret_cast<HWND>(this->winId()), 1, MOD_CONTROL | MOD_ALT, 'S');
        RegisterHotKey(reinterpret_cast<HWND>(this->winId()), 1, modifier1 | modifier2, hotkey);
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Kürzel registriert.";

}

void MainWindow::closeEvent(QCloseEvent* event)
{
        //Kürzel wieder entfernen.
        //Bitte verlassen Sie diesen Raum so, wie Sie ihn vorzufinden wünschen.
        UnregisterHotKey(reinterpret_cast<HWND>(this->winId()), 1);
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Kürzel entfernt.";
        QMainWindow::closeEvent(event);
}

bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long long* result)
{
        qDebug() << "nativeEvent aufgerufen";

        Q_UNUSED(eventType);

        // Reaktion auf Kürzel
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == 1)
        {
            qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Kürzel gedrückt.";
            // Kürzel wurde gedrückt, rufe Funktion auf
            on_pushButton_clicked();
        }

        return false;
}

//Die Funktion zum Speichern der Einstellungen
void MainWindow::saveSettings() {
        //Pfad zur neuen .ini-Datei festlegen
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Alsweider", "SpotiQuit");

        // Checkbox-Einstellungen speichern
        settings.setValue("checkBoxGentleClosing", ui->checkBoxGentleClosing->isChecked());
        settings.setValue("resumePlaybackCheckBox", ui->resumePlaybackCheckBox->isChecked());
        settings.setValue("checkBox", ui->checkBox->isChecked());

        // Radio-Button-Einstellungen speichern
        settings.setValue("radioButton", ui->radioButton->isChecked());
        settings.setValue("radioButton_2", ui->radioButton_2->isChecked());

        // Eingabefeld-Einstellungen speichern
        settings.setValue("lineEdit", ui->lineEdit->text());

        // Wert der spinBox speichern
        settings.setValue("spinBoxValue", ui->spinBox->value());
}

// Einstellungen laden
void MainWindow::loadSettings() {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Alsweider", "SpotiQuit");


        // Checkbox-Einstellungen wiederherstellen
        ui->checkBoxGentleClosing->setChecked(settings.value("checkBoxGentleClosing", false).toBool());

        // Überprüfen, ob die Einstellung für resumePlaybackCheckBox existiert
        if (settings.contains("resumePlaybackCheckBox")) {
            ui->resumePlaybackCheckBox->setChecked(settings.value("resumePlaybackCheckBox").toBool());

            //Radioknöpfe entsprechend der Checkbox regulieren
            if (ui->resumePlaybackCheckBox->isChecked()){
            ui->radioButton->setEnabled(true);
            ui->radioButton_2->setEnabled(true);
            } else {
            ui->radioButton->setEnabled(false);
            ui->radioButton_2->setEnabled(false);
            }

        } else {
            // Wenn die Einstellung für resumePlaybackCheckBox nicht existiert, Checkbox auf checked setzen
            ui->resumePlaybackCheckBox->setChecked(true); // Hier wird die Checkbox standardmäßig aktiviert
            ui->radioButton->setEnabled(true);
            ui->radioButton_2->setEnabled(true);
            ui->radioButton_2->setChecked(true);
        }

        //Überprüfung der checkBox zum Minimieren
        if (settings.contains("checkBox")){
            ui->checkBox->setChecked(settings.value("checkBox").toBool());
        } else{
            //Wenn checkBox nicht in den Einstellungen ist, Standardwert setzen
            ui->checkBox->setChecked(true);
        }

        // Radio-Button-Einstellungen wiederherstellen
        ui->radioButton->setChecked(settings.value("radioButton", false).toBool());
        ui->radioButton_2->setChecked(settings.value("radioButton_2", false).toBool());

        // Eingabefeld-Einstellungen wiederherstellen
        QString lineEditText = settings.value("lineEdit", "").toString();
        if (!lineEditText.isEmpty()) {
            ui->lineEdit->setText(lineEditText);
        } else {
            // Wenn der Text in den Einstellungen nicht gefunden wurde, Standardpfad setzen
            pfadSetzen();
        }

        // Wert der spinBox wiederherstellen
        int spinBoxValue = settings.value("spinBoxValue", -1).toInt();
        if (spinBoxValue != -1) {
            ui->spinBox->setValue(spinBoxValue);
        } else {
            // Wenn der Wert nicht in den Einstellungen gespeichert ist, Standardwert setzen
            ui->spinBox->setValue(100);
        }
}

void MainWindow::on_pushButtonSaveSettings_clicked()
{
        saveSettings();
        ui->label->setText("Settings saved");
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") + " - Settings saved";
        // Pfad zur INI-Datei abrufen
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Alsweider", "SpotiQuit");
        QString iniFilePath = settings.fileName(); // Dies gibt den vollständigen Pfad zur INI-Datei zurück

        //Aufspringfenster zeigt ehrenhaft den Pfad zur Einstellungsdatei an.
        QMessageBox msgBox;
        msgBox.setWindowTitle("Settings location");
        msgBox.setText("The settings file is located at:\n" + iniFilePath);
        msgBox.setStyleSheet("QLabel { color: rgb(216, 184, 49); font: 700 9pt Arial; }"
                             "QMessageBox {setStandardButtons(QMessageBox::Ok); }"
                             "QMessageBox {setDefaultButton(QMessageBox::Ok); }"
                             );

        //Kopierbaren Pfad als Text beifügen
        msgBox.setDetailedText(iniFilePath);
        msgBox.exec();
}

//Das bleibt alles so, wies hier ist und da wird sich hier nichts dran rütteln,
//egal ob du hier bist und nich!
void MainWindow::on_pushButtonReset_clicked()
{
        ui->resumePlaybackCheckBox->setChecked(true);
        ui->radioButton->setEnabled(true);
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
        ui->checkBoxGentleClosing->setChecked(false);
        ui->checkBox->setChecked(true);
        ui->spinBox->setValue(100);
        pfadSetzen();
        ui->label->setText("Settings reset");
}

