#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ArkPad");

    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(showCursorPos()));
    connect(ui->textEdit, &QTextEdit::textChanged, [=](){
        if (ui->textEdit->document()->isModified())
        {
            QString edited = "*";
            setWindowTitle("ArkPad -- " + FileName + edited);
            qDebug() << "modify " << FileName;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewFile_triggered()
{
    QMessageBox SaveFileMessage, SaveAsFileMessage;

    SaveFileMessage.setMinimumSize(5000, 200);
    SaveFileMessage.setWindowTitle("Save file");
    SaveFileMessage.setText("Wanted to Save the File");
    SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
    SaveFileMessage.setIcon(QMessageBox::Question);
    SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    SaveAsFileMessage.setMinimumSize(5000, 200);
    SaveAsFileMessage.setWindowTitle("Save file");
    SaveAsFileMessage.setText("Wanted to Save As the File");
    SaveAsFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save as else no to Clear workspace.");
    SaveAsFileMessage.setIcon(QMessageBox::Question);
    SaveAsFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if (!ui->textEdit->document()->isEmpty())
    {
        if (ui->textEdit->document()->isModified())
        {
            // qDebug() << "modified " << FileName;
            if (FileName.isEmpty())
            {
                int saveas = SaveAsFileMessage.exec();
                if (saveas == QMessageBox::Yes)
                {
                    on_actionSaveAs_triggered();
                    file_path = "";
                    ui->textEdit->setText("");
                }
                if (saveas == QMessageBox::No)
                {
                    file_path = "";
                    ui->textEdit->setText("");
                }
            }
            else
            {
                int save = SaveFileMessage.exec();
                if (save == QMessageBox::Yes)
                {
                    on_actionSave_triggered();
                    file_path = "";
                    ui->textEdit->setText("");
                }
                if (save == QMessageBox::No)
                {
                    file_path = "";
                    ui->textEdit->setText("");
                }
            }
        }
        else
        {
            on_QuitFile_triggered();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    qDebug() << "saved" << FileName;
    if (FileName.isEmpty())
    {
        on_actionSaveAs_triggered();
    }

    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty())
    {
        FileName = QFileDialog::getSaveFileName(this, "Save");
        if (FileName.isEmpty())
            return;
        currentFile = FileName;
    }
    else
    {
        FileName = currentFile;
    }
    QFile file(FileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();

    setWindowTitle("ArkPad -- " + FileName);
    ui->textEdit->document()->setModified(false);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save As");
    if (filename.isEmpty())
    {
        return;
    }

    QFile file(filename);

    // Open the filename
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QMessageBox::warning(this, "..", "File not open");
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText() << endl;

    file.close();
}

void MainWindow::on_actionOpen_File_triggered()
{

    if (ui->textEdit->document()->isModified())
    {
        QMessageBox SaveFileMessage;

        SaveFileMessage.setMinimumSize(5000, 200);
        SaveFileMessage.setWindowTitle("Save file");
        SaveFileMessage.setText("Wanted to Save the File");
        SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
        SaveFileMessage.setIcon(QMessageBox::Question);
        SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int save = SaveFileMessage.exec();
        if (save == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        if (save == QMessageBox::Cancel)
        {
            return;
        }
        if (save == QMessageBox::No)
        {
            ui->textEdit->document()->setModified(false);
        }
    }

    // QTextCharFormat
    // save the file to disk

    QString filename = QFileDialog::getOpenFileName(this, "Open File");

    if (filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString line;
    line = in.readLine();

    while (!line.isNull())
    {
        fileContent.append(line);
        line = in.readLine();
    }

    // fileSize = file.size();

    QFileInfo fi(file);
    FileName = fi.fileName();
    setWindowTitle("ArkPad -- " + FileName);

    qDebug() << "opened" << FileName;
    ui->textEdit->setPlainText(fileContent);
    fileContent.clear();

    ui->textEdit->document()->setModified(false);
    setWindowTitle("ArkPad -- " + FileName);

    file.close();

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textModified()));
}

void MainWindow::on_QuitFile_triggered()
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox SaveFileMessage;

        SaveFileMessage.setMinimumSize(5000, 200);
        SaveFileMessage.setWindowTitle("Save file");
        SaveFileMessage.setText("Wanted to Save the File");
        SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
        SaveFileMessage.setIcon(QMessageBox::Question);
        SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        int save = SaveFileMessage.exec();
        if (save == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        if (save == QMessageBox::No)
        {
            QFile file(FileName);
            file.close();
            ui->textEdit->clear();
            ui->textEdit->document()->setModified(false);
            qDebug() << "Closed " << FileName;
            setWindowTitle("ArkPad");
        }
    }
    else
    {
        QFile file(FileName);

        file.close();
        ui->textEdit->clear();
        ui->textEdit->document()->setModified(false);
        qDebug() << "Closed " << FileName;
        FileName = "";
        setWindowTitle("ArkPad");
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "This is a simple notepad application created by Arpan Kumar \n\n\nThis is version 1.0.0");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionPrint_triggered()
{
    // Allows for interacting with printer
    QPrinter printer;

    // You'll put your printer name here
    printer.setPrinterName("Printer Name");

    // Create the print dialog and pass the name and parent
    QPrintDialog pDialog(&printer, this);

    if (pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }

    // Send the text to the printer
    ui->textEdit->print(&printer);
}

void MainWindow::showCursorPos()
{
    int line = ui->textEdit->textCursor().blockNumber() + 1;
    int col = ui->textEdit->textCursor().positionInBlock();
    ui->statusbar->showMessage(QString("Ln %1, Col %2").arg(line).arg(col));
}

void MainWindow::textModified()
{
    if (ui->textEdit->document()->isModified())
    {
        QString edited = "*";
        setWindowTitle("ArkPad -- " + FileName + edited);
        qDebug() << "modify " << FileName;
    }
}
