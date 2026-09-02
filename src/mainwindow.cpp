#include "common.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    defaultCitesPath = qApp->applicationDirPath() + "/quotes";
    quote = "<No cite>";

    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);
    m_newCiteButton = new QPushButton(tr("New"), this);
    m_saveCiteButton = new QPushButton(tr("Save"), this);
    m_previewCiteButton = new QPushButton(tr("Preview"), this);
    m_currentCiteButton = new QPushButton(tr("Current"), this);
    m_nextCiteButton = new QPushButton(tr("Next"), this);
    m_quitButton = new QPushButton(tr("Quit"), this);

    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(m_newCiteButton);
    buttonsLayout->addWidget(m_saveCiteButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(m_previewCiteButton);
    buttonsLayout->addWidget(m_currentCiteButton);
    buttonsLayout->addWidget(m_nextCiteButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(m_quitButton);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_textEdit);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    const QIcon iconQuit = QIcon(":/images/exit.png");
    QAction* actionQuit = new QAction(tr("Quit"), this);
    actionQuit->setIcon(iconQuit);
    connect(actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    const QIcon iconViewQuote = QIcon(":/images/citorius.png");
    QAction* viewQuote = new QAction(tr("View quote"), this);
    viewQuote->setIcon(iconViewQuote);
    connect(viewQuote, &QAction::triggered, this, &MainWindow::slotOpenWindow);

    trayMenu = new QMenu(this);
    trayMenu->addAction(viewQuote);
    trayMenu->addAction(actionQuit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip(tr("Citorius"));
    trayIcon->setIcon(QPixmap(":/images/citorius.png"));

    trayIcon->show();

    setVisible(true);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::slotDoubleClickOpen);
    connect(m_newCiteButton, &QPushButton::clicked, this, &MainWindow::slotNewCite);
    connect(m_saveCiteButton, &QPushButton::clicked, this, &MainWindow::slotSaveCite);
    connect(m_previewCiteButton, &QPushButton::clicked, this, &MainWindow::slotPreviewCite);
    connect(m_currentCiteButton, &QPushButton::clicked, this, &MainWindow::slotCurrentCite);
    connect(m_nextCiteButton, &QPushButton::clicked, this, &MainWindow::slotNextCite);

    m_previewCiteButton->setDisabled(true);

    dir.setPath(defaultCitesPath);

    for(QFileInfo item : dir.entryInfoList())
    {
        if(item.isFile())
        {
            listQuotes << item.absoluteFilePath();
        }
    }

    generator = QRandomGenerator::system();

    if(listQuotes.isEmpty())
    {
        m_nextCiteButton->setDisabled(true);
    }
    else
    {
        numQuote = generator->bounded(listQuotes.size());
        quote = loadFileToString(listQuotes.at(numQuote));
        stackQuotes.push(numQuote);
    }

    m_textEdit->setText(quote);
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotOpenWindow()
{
    setVisible(true);
}

void MainWindow::slotDoubleClickOpen(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
        setVisible(true);
}

void MainWindow::slotNewCite()
{
    m_textEdit->clear();
    m_textEdit->setReadOnly(false);
}

void MainWindow::slotSaveCite()
{
    QString nameFile = QFileDialog::getSaveFileName(nullptr, tr("Задать имя файла"), defaultCitesPath, "*.txt");

    if(nameFile.isEmpty())
    {
        return;
    }

    QString cite = m_textEdit->toPlainText();

    if(cite.isEmpty())
    {
        return;
    }

    if(!saveStringToFile(cite, nameFile))
    {
        QMessageBox::warning(nullptr, tr("Ошибка"), tr("Ошибка сохранения файла"));
    }
}

void MainWindow::slotPreviewCite()
{
    numQuote = stackQuotes.pop();
    numQuote = stackQuotes.top();
    quote = loadFileToString(listQuotes.at(numQuote));
    m_textEdit->setText(quote);
    if(stackQuotes.count() == 1)
    {
        m_previewCiteButton->setDisabled(true);
    }
}

void MainWindow::slotCurrentCite()
{
    m_textEdit->setReadOnly(true);
    m_textEdit->setText(quote);
}

void MainWindow::slotNextCite()
{
    numQuote = generator->bounded(listQuotes.size());
    quote = loadFileToString(listQuotes.at(numQuote));
    m_textEdit->setText(quote);
    stackQuotes.push(numQuote);
    m_previewCiteButton->setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(trayIcon->isVisible())
        hide();
}
