#ifndef CALLOUTPANEL_H
#define CALLOUTPANEL_H

#include <QWidget>

class CalloutPanel : public QWidget {
public:
    explicit CalloutPanel(QWidget * parent = nullptr);

    virtual void focusOutEvent(QFocusEvent* e) override;
    virtual void callout();

protected:
    void hideOnFocusOut();
};

#endif	 // CALLOUTPANEL_H
