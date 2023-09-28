#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdlib>
#include <windows.h>
#include <QProcess>
#include <QStandardPaths>
#include <QMessageBox>
#include <QClipboard>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Für das Tastenkürzel die 3 überschriebenen Methoden
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long long* result) override;

    //Zum Speichern der Einstellungen
    void saveSettings();
    void loadSettings();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_resumePlaybackCheckBox_clicked();

    void on_playPauseButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButtonSaveSettings_clicked();

    void on_pushButtonReset_clicked();

private:
    Ui::MainWindow *ui;
    bool IsSpotifyOpen();
    void pfadSetzen();



};
#endif // MAINWINDOW_H
