#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QComboBox>
#include <QTimer>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>
#include <QStringList>
#include <QInputDialog>

#include "event.h"
#include "add_new_event_dialog.h"


namespace Ui {
class manager;
}

class manager : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit manager(QWidget *parent = 0);
    ~manager();
    
protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject *object, QEvent *event);
private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void update();
    void addNew();
    void deleteTask();
    void openEvent(QListWidgetItem* event);
private:
    void createActions();
    void createTrayIcon();
    void createMainToolBar();
    void createMenuBar();

    void loadEventsFile();
    void addNewEvent();
    void loadEventToFile(Event* newEvent);
    bool isAnyEventActive();

    QAction* addNewAction;
    QAction* deleteAction;
    QAction* quitAction;

    Ui::manager *ui;
    QSystemTrayIcon* trayIcon;
    QTimer* timer;

    QList<Event*> listOfEvents;
};

#endif // MANAGER_H
