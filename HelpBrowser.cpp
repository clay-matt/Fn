//
/////////////////////////////////////////////////////////

#include <QtGui>
#include <QDesktopServices>

#include "HelpBrowser.h"

/////////////////////////////////////////////////////////

HelpBrowser::HelpBrowser(const QString & page, QWidget *parent) :
    QWidget(parent)
{

    webView = new QWebView(this);

    backButton = new QPushButton;
    backButton->setProperty("back",true);
    forwardButton = new QPushButton;
    forwardButton->setProperty("forward",true);

    findEdit = new QLineEdit;
    findEdit->setPlaceholderText(tr("find"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(forwardButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(findEdit);
    buttonLayout->setContentsMargins(16,0,16,0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(webView);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(backButton, SIGNAL(clicked()),
            webView, SLOT(back()));
    connect(forwardButton, SIGNAL(clicked()),
            webView, SLOT(forward()));
    connect(findEdit,SIGNAL(textChanged(const QString &)),
            this,SLOT(findText(const QString &)));

    connect(webView, SIGNAL(titleChanged(const QString &)),
            this, SLOT(updateWindowTitle(const QString &)));
    connect(webView, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(externalLinkClicked(const QUrl &)));

    webView->load(QUrl(page));
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);

}

void HelpBrowser::showPage(const QString &page)
{

    HelpBrowser *browser = new HelpBrowser(page);
    browser->resize(600,550);
    browser->show();

}

/////////////////////////////////////////////////////////
//SLOTS

void HelpBrowser::externalLinkClicked(const QUrl &url)
{

    QDesktopServices::openUrl(url);

}

void HelpBrowser::findText(const QString &text)
{

    webView->findText(QString(""),QWebPage::HighlightAllOccurrences); // clears previous highlights
    webView->findText(text,QWebPage::HighlightAllOccurrences);

}

void HelpBrowser::updateWindowTitle(const QString &title)
{

    setWindowTitle(tr("Help: %1").arg(title));

}

