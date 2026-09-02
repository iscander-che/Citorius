#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDir>
#include <QString>
#include <QRandomGenerator>
#include <QStringList>
#include <QStack>
#include <QTextEdit>
#include <QPushButton>
#include <QCloseEvent>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString defaultCitesPath;

    QTextEdit* m_textEdit;
    QPushButton* m_newCiteButton;
    QPushButton* m_saveCiteButton;
    QPushButton* m_previewCiteButton;
    QPushButton* m_currentCiteButton;
    QPushButton* m_nextCiteButton;
    QPushButton* m_quitButton;

    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    QRandomGenerator* generator;
    QDir dir;
    QStringList listQuotes;
    int numQuote;
    QStack<int> stackQuotes;
    QString quote;

private slots:
    void slotOpenWindow();
    void slotDoubleClickOpen(QSystemTrayIcon::ActivationReason reason);

    void slotNewCite();
    void slotSaveCite();
    void slotPreviewCite();
    void slotCurrentCite();
    void slotNextCite();

protected:
    virtual void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
