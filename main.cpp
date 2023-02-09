#include "mainpanel.h"
#include "tabpanel.h"

#include <QHotkey>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QFontDatabase>
#include <QApplication>

void createTrayIcon(QApplication* app, CalloutPanel* w);

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/font/Source Code Pro for Powerline.otf");

    MainPanel mainPanel;
    TabPanel  tabPanel;

    {	//! Create tray icon
        auto actionHistory = new QAction(QObject::tr("&History"));
        auto actionQuit	   = new QAction(QObject::tr("&Quit"));
        auto trayMenu	   = new QMenu(&mainPanel);
        trayMenu->addAction(actionHistory);
        trayMenu->addAction(actionQuit);

        auto trayIcon = new QSystemTrayIcon(&mainPanel);
        trayIcon->setIcon(QIcon(":/icon/app.ico"));
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();

        QObject::connect(actionHistory, &QAction::triggered, &tabPanel, &CalloutPanel::callout);
        QObject::connect(actionQuit, &QAction::triggered, &app, &QApplication::quit);

        QObject::connect(trayIcon,
                         &QSystemTrayIcon::activated,
                         &mainPanel,
                         [&self = mainPanel](QSystemTrayIcon::ActivationReason reason) {
                             if (reason == QSystemTrayIcon::DoubleClick) {
                                 self.callout();
                             }
                         });
    }

    tabPanel.bind(&mainPanel.weightedHistory);
    QObject::connect(
        &tabPanel, &TabPanel::select, &mainPanel, [&self = mainPanel](const QString& item) {
            self.switchDesktopTo(item);
        });

    QHotkey calloutShortcut(QKeySequence("Ctrl+Shift+L"), true);
    QObject::connect(&calloutShortcut, &QHotkey::activated, &tabPanel, &CalloutPanel::callout);

    return app.exec();
}
