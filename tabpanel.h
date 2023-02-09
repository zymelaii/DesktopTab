#ifndef TABPANEL_H
#define TABPANEL_H

#include "calloutpanel.h"

#include <QStringListModel>
#include <QListView>
#include <QMap>

class TabPanel : public CalloutPanel {
    Q_OBJECT

public:
    explicit TabPanel(QWidget* parent = nullptr);

    virtual void focusInEvent(QFocusEvent* e) override;
    virtual void callout() override;

    void setupUi();
    void bind(QMap<QString, size_t>* target);

signals:
    void select(const QString &item);

private:
    QStringListModel*	   model;
    QListView*			   lvHistory;
    QMap<QString, size_t>* records;
};

#endif	 // TABPANEL_H
