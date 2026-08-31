#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>
#include <QMenuBar>
#include <QAction>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineView>

class BrowserWindow : public QMainWindow {
    Q_OBJECT

public:
    BrowserWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        tabWidget = new QTabWidget(this);
        tabWidget->setTabsClosable(true);
        setCentralWidget(tabWidget);

        QToolBar *toolbar = addToolBar("Navigation");
        urlBar = new QLineEdit(this);
        QAction *newTabAction = new QAction("New Tab", this);
        QAction *newIncognitoAction = new QAction("New Incognito Tab", this);

        toolbar->addWidget(urlBar);
        toolbar->addAction(newTabAction);
        toolbar->addAction(newIncognitoAction);

        QMenu *fileMenu = menuBar()->addMenu("&File");
        fileMenu->addAction(newTabAction);
        fileMenu->addAction(newIncognitoAction);

        connect(newTabAction, &QAction::triggered, this, [=]() {
            createTab(false);
        });

        connect(newIncognitoAction, &QAction::triggered, this, [=]() {
            createTab(true);
        });

        connect(urlBar, &QLineEdit::returnPressed, this, [=]() {
            QWebEngineView *currentView = getCurrentWebView();
            if (currentView) {
                QUrl url = QUrl::fromUserInput(urlBar->text());
                if (url.isValid())
                    currentView->setUrl(url);
            }
        });

        connect(tabWidget, &QTabWidget::tabCloseRequested, this, [=](int index) {
            QWidget *tab = tabWidget->widget(index);
            tabWidget->removeTab(index);
            delete tab;
        });

        connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            QWebEngineView *currentView = getCurrentWebView();
            if (currentView)
                urlBar->setText(currentView->url().toString());
        });

        createTab(false);  // Open default tab on startup
        setWindowTitle("My Cool Browser");
        resize(1000, 700);
    }

private:
    QTabWidget *tabWidget;
    QLineEdit *urlBar;

    void createTab(bool incognito = false) {
        QWebEngineProfile *profile;
    
        if (incognito) {
            // Off-the-record profile
            profile = new QWebEngineProfile(this);
        } else {
            profile = QWebEngineProfile::defaultProfile();
        }
    
        QWebEngineView *view = new QWebEngineView(this);
        QWebEnginePage *page = new QWebEnginePage(profile, view);
    
        view->setPage(page);
        view->setUrl(QUrl("https://www.qt.io"));
    
        int index = tabWidget->addTab(
            view,
            incognito ? "Incognito" : "New Tab"
        );
    
        tabWidget->setCurrentIndex(index);
    
        connect(view, &QWebEngineView::urlChanged,
                this, [=](const QUrl &url) {
            if (tabWidget->currentWidget() == view) {
                urlBar->setText(url.toString());
    
                QString title = url.host();
    
                if (title.isEmpty()) {
                    title = incognito ? "Incognito" : "New Tab";
                }
    
                tabWidget->setTabText(index, title);
            }
        });
    }

    QWebEngineView* getCurrentWebView() const {
        return qobject_cast<QWebEngineView *>(tabWidget->currentWidget());
    }
};

#include "Main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BrowserWindow window;
    window.show();

    return app.exec();
}
