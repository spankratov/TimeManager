#include "manager.h"
#include "ui_manager.h"

manager::manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::manager)
{
    ui -> setupUi(this);
    ui -> taskList -> installEventFilter(this); // for context menu
    ui -> taskList -> setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui -> taskList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openEvent(QListWidgetItem*)));

    createActions();
    createTrayIcon();
    createMainToolBar();
    createMenuBar();
    loadEventsFile();

    for(QList<Event*>::iterator i = listOfEvents.begin(); i != listOfEvents.end(); ++i)
        ui -> taskList-> addItem(*i);

    trayIcon -> show();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    if(isAnyEventActive())
    {
        QTime nextReminding(QTime::currentTime().hour(), QTime::currentTime().minute() + 1, 0, 0);
        timer -> start(QTime::currentTime().msecsTo(nextReminding));
    }
}

void manager::closeEvent(QCloseEvent* event)
{
    this -> hide();
    event -> ignore();
}

void manager::resizeEvent(QResizeEvent* event)
{
    if(event -> oldSize().isValid())
        ui -> taskList -> resize( ui -> taskList -> size() + event -> size() - event -> oldSize() );
}

bool manager::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui -> taskList)
    {
            if (event -> type() == QEvent::ContextMenu)
            {
                QContextMenuEvent *menuEvent = static_cast<QContextMenuEvent*>(event);
                QMenu* contextMenu = new QMenu(this);
                contextMenu -> addAction(addNewAction);
                contextMenu -> addAction(deleteAction);
                contextMenu -> addAction(quitAction);
                contextMenu -> popup(menuEvent -> globalPos());
                return true;
            }
            else
                return false;
    }
    else
        return QMainWindow::eventFilter(object, event);
}

void manager::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this -> show();
        break;
    case QSystemTrayIcon::Context:
        (trayIcon -> contextMenu()) -> show();
        break;
    default:
        ;
    }
}

void manager::update()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    if(!isAnyEventActive())
    {
        timer -> stop();
        return;
    }
    int nextInterval = -1;
    // Events
    QString eventMessage("");
    for(QList<Event*>::iterator i = listOfEvents.begin(); i != listOfEvents.end(); ++i)
    {
        if(     (*i) -> isActive() &&
                ((*i) -> nextTimeToRemind().date() == currentDateTime.date()) &&
                ((*i) -> nextTimeToRemind().time().hour() == currentDateTime.time().hour()) &&
                ((*i) -> nextTimeToRemind().time().minute() == currentDateTime.time().minute())
          )
        {
            eventMessage.append('\n');
            eventMessage.append((*i) -> summary());
            eventMessage.append(' ');
            eventMessage.append((*i) -> startingTime().toString());
            eventMessage.append(" — ");
            eventMessage.append((*i) -> endingTime().toString());
            (*i) -> moveTimeToRemind();
        }
        if((*i) -> isActive())
        {
            int currentInterval = currentDateTime.secsTo((*i) -> nextTimeToRemind()) * 1000;
            if((nextInterval == -1) || (currentInterval < nextInterval))
                nextInterval = currentInterval;
        }

    }
    if(!eventMessage.isEmpty())
        trayIcon -> showMessage("Reminder about your events:\n", eventMessage,
                            QSystemTrayIcon::Information, 100000);
    // Habits

    // Hourlies

    // Tasks
    if(nextInterval == -1)
        nextInterval = 1000;
    timer -> setInterval(nextInterval);
}

void manager::addNew()
{
    QStringList taskTypes;
    taskTypes << "Event" << "Habit" << "Hourly" << "Task" << "ToDo";
    bool ok;
    QString taskType = QInputDialog::getItem(this, "Add new task", "Type:", taskTypes, 0, false, &ok);
    if(ok)
    {
        if(taskType == "Event")
        {
            addNewEvent();
        }
    }
}

void manager::deleteTask()
{
    QList<QListWidgetItem*> selectedList = ui -> taskList -> selectedItems();
    for(QList<QListWidgetItem*>::iterator i = selectedList.begin(); i != selectedList.end(); ++i)
    {
        ui -> taskList -> takeItem(ui -> taskList -> row(*i));
        listOfEvents.removeAt(listOfEvents.indexOf(dynamic_cast<Event*>(*i)));
    }
    ui -> taskList -> update();
    QFile eventsFile("events");
    eventsFile.open(QIODevice::WriteOnly);
    eventsFile.close();
    for(QList<Event*>::iterator i = listOfEvents.begin(); i != listOfEvents.end(); ++i)
        loadEventToFile(*i);
}

void manager::openEvent(QListWidgetItem *event)
{
    Event* thisEvent = dynamic_cast<Event*>(event);
    AddNewEventDialog* addNewEventDialog = new AddNewEventDialog(this);
    bool isNewEventHere;
    addNewEventDialog -> editEvent(isNewEventHere, thisEvent);
    ui -> taskList -> update();
    if(isNewEventHere)
    {
        timer -> stop();
        QTime nextReminding(QTime::currentTime().hour(), QTime::currentTime().minute() + 1, 0, 0);
        timer -> start(QTime::currentTime().msecsTo(nextReminding));

        QFile eventsFile("events");
        eventsFile.open(QIODevice::WriteOnly);
        eventsFile.close();
        for(QList<Event*>::iterator i = listOfEvents.begin(); i != listOfEvents.end(); ++i)
            loadEventToFile(*i);
    }
}

void manager::createActions()
{
    addNewAction = new QAction(QIcon("icons\\AddNew.ico"), "Add new...", this);
    connect(addNewAction, SIGNAL(triggered()), this, SLOT(addNew()));

    deleteAction = new QAction(QIcon("icons\\Delete.ico"), "Delete", this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteTask()));

    quitAction = new QAction(QIcon("icons\\Quit.ico"), "Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void manager::createTrayIcon()
{
    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction(addNewAction);
    trayIconMenu -> addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon -> setContextMenu(trayIconMenu);
    trayIcon -> setIcon(QIcon("icons\\TimeManager.ico"));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                 SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void manager::createMainToolBar()
{
    ui -> mainToolBar -> addAction(addNewAction);
    ui -> mainToolBar -> addAction(deleteAction);
    ui -> mainToolBar -> addAction(quitAction);
}

void manager::createMenuBar()
{
    QMenu* fileMenu = menuBar() -> addMenu("&File");
    QMenu* editMenu = menuBar() -> addMenu("&Edit");
    fileMenu -> addAction(addNewAction);
    editMenu -> addAction(deleteAction);
    fileMenu -> addAction(quitAction);
}

void manager::loadEventsFile()
{
    QFile eventsFile("events");
    if(eventsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        try
        {
            QTextStream eventsFileStream(&eventsFile);
            QString summary, comment, priority;
            qint64 startingTime, endingTime, whenStartToRemind, intervalOfReminding;
            QString remindDuringEvent;
            TMPriority pr;
            while(!eventsFileStream.atEnd())
            {
                summary = eventsFileStream.readLine();
                if(summary.isEmpty())
                    throw 1;

                comment = eventsFileStream.readLine();

                priority = eventsFileStream.readLine();
                if(priority == "None")
                    pr = None;
                else if(priority == "Low")
                    pr = Low;
                else if(priority == "Medium")
                    pr = Medium;
                else if(priority == "High")
                    pr = High;
                else
                    throw 1;

                eventsFileStream >> startingTime;
                QDateTime sT = QDateTime::fromMSecsSinceEpoch(startingTime);
                if(!sT.isValid())
                    throw 1;
                if((sT.time().second() != 0) || (sT.time().msec() != 0))
                    throw 1;

                eventsFileStream >> endingTime;
                QDateTime eT = QDateTime::fromMSecsSinceEpoch(endingTime);
                if(!eT.isValid())
                    throw 1;
                if((eT.time().second() != 0) || (eT.time().msec() != 0))
                    throw 1;

                if(sT > eT)
                    throw 1;

                eventsFileStream >> whenStartToRemind;
                QDateTime wSTR = QDateTime::fromMSecsSinceEpoch(whenStartToRemind);
                if(!wSTR.isValid())
                    throw 1;
                if((wSTR.time().second() != 0) || (wSTR.time().msec() != 0))
                    throw 1;

                eventsFileStream >> intervalOfReminding;
                if(intervalOfReminding <= 0)
                    throw 1;
                if(intervalOfReminding % 1000 * 60 != 0)
                    throw 1;

                eventsFileStream.skipWhiteSpace();

                remindDuringEvent = eventsFileStream.readLine();
                bool rDE;
                if(remindDuringEvent == "true")
                    rDE = true;
                else if(remindDuringEvent == "false")
                    rDE = false;
                else
                    throw 1;

                eventsFileStream.readLine();

                Event* newEvent = new Event(summary, comment, pr, sT, eT, wSTR, intervalOfReminding, rDE);
                listOfEvents.append(newEvent);
            }
        }catch(...)
        {
            QMessageBox cantReadEventsFile(QMessageBox::Critical,
                                           "Error!", "Can not read the events file. Do you want to rewrite this file?",
                                           QMessageBox::NoButton, this);
            QPushButton *rewriteEventsFileButton =
                    cantReadEventsFile.addButton("Rewrite the file", QMessageBox::NoRole);
            QPushButton *closeApplicationButton = cantReadEventsFile.addButton("Close application", QMessageBox::NoRole);
            cantReadEventsFile.exec();
            if(cantReadEventsFile.clickedButton() == rewriteEventsFileButton)
            {
                eventsFile.close();
                eventsFile.open(QIODevice::WriteOnly);
            }
            else
            {
                eventsFile.close();
                QTimer* closingTimer = new QTimer;
                closingTimer -> setSingleShot(true);
                connect(closingTimer, SIGNAL(timeout()), qApp, SLOT(quit()));
                closingTimer -> start(0);
            }
        }

    }
    else
        eventsFile.open(QIODevice::WriteOnly);
    eventsFile.close();
}

void manager::addNewEvent()
{
    AddNewEventDialog* addNewEventDialog = new AddNewEventDialog(this);
    bool isNewEventHere;
    Event* newEvent = addNewEventDialog -> getNewEvent(isNewEventHere);
    if(isNewEventHere)
    {
        listOfEvents.append(newEvent);
        if(isAnyEventActive())
        {
            QTime nextReminding(QTime::currentTime().hour(), QTime::currentTime().minute() + 1, 0, 0);
            timer -> start(QTime::currentTime().msecsTo(nextReminding));
        }
        ui -> taskList -> addItem(listOfEvents.last());
        loadEventToFile(newEvent);
    }
}

void manager::loadEventToFile(Event *newEvent)
{
    QFile eventsFile("events");
    eventsFile.open(QIODevice::Append | QIODevice::Text);
    QTextStream eventsFileStream(&eventsFile);
    eventsFileStream << newEvent -> summary() << endl << newEvent -> comment() << endl;
    TMPriority priority = newEvent -> priority();
    if(priority == None)
        eventsFileStream << "None" << endl;
    else if(priority == Low)
        eventsFileStream << "Low" << endl;
    else if(priority == Medium)
        eventsFileStream << "Medium" << endl;
    else if(priority == High)
        eventsFileStream << "High" << endl;
    eventsFileStream << newEvent -> startingTime().toMSecsSinceEpoch() << endl
                     << newEvent -> endingTime().toMSecsSinceEpoch() << endl
                     << newEvent -> whenStartToRemind().toMSecsSinceEpoch() << endl
                     << newEvent -> intervalOfReminding() << endl;
    if(newEvent -> remindDuringEvent())
        eventsFileStream << "true" << endl;
    else
        eventsFileStream << "false" << endl;
    eventsFileStream << endl;

    eventsFile.close();
}

bool manager::isAnyEventActive()
{
    if(listOfEvents.isEmpty())
        return false;
    for(QList<Event*>::iterator i = listOfEvents.begin(); i != listOfEvents.end(); ++i)
        if((*i) -> isActive())
            return true;
    return false;
}

manager::~manager()
{
    delete ui;
}
