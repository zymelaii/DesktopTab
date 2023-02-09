#ifndef MAINPANEL_H
#define MAINPANEL_H

#include "calloutpanel.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QMap>
#include <ranges>

class MainPanel : public CalloutPanel {
public:
    MainPanel(QWidget* parent = nullptr);

    virtual void callout() override;

    void setupUi();
    bool switchDesktopTo(const QString& path);

public:
    QMap<QString, size_t> weightedHistory;

private:
    const QString appHome;

    QLabel* lbCurrentPath;
};
#endif	 // MAINPANEL_H
