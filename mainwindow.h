#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCharFormat>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_QuitFile_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionNewFile_triggered();

    void on_actionSaveAs_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_File_triggered();

    void on_actionPrint_triggered();

    void showCursorPos();

    void textModified();


private:
    Ui::MainWindow *ui;

    QString file_path, fileContent, FileName;
    QString currentFile;
    //qint64 fileSize;
};
#endif // MAINWINDOW_H
