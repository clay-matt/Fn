// Header file for class HelpBrowser
// inherits QWidget

// author Matt Clay
// version 110420

/////////////////////////////////////////////////////////

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QtWebKit>
#include <QWidget>

class QLineEdit;
class QPushButton;

class HelpBrowser : public QWidget
{

    Q_OBJECT

public:
    HelpBrowser(const QString & page, QWidget *parent = 0);

    static void showPage(const QString & page);

private slots:
    void externalLinkClicked(const QUrl &url);
    void findText(const QString &text);
    void updateWindowTitle(const QString &title);

private:
    QLineEdit *findEdit;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QWebView *webView;

};

#endif // HELPBROWSER_H
