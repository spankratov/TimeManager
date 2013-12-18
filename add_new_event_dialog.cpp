#include "add_new_event_dialog.h"

void AddNewEventDialog::okayButtonClicked()
{
    try
    {
        QString summary = summaryLineEdit -> text();
        if(summary.isEmpty())
            throw QString("You did not enter the summary.");
        QString comment = commentTextEdit -> toPlainText();
        TMPriority priority;

        QString pr = priorityComboBox -> currentText();
        if(pr == "None")
            priority = None;
        else if(pr == "Low")
            priority = Low;
        else if(pr == "Medium")
            priority = Medium;
        else if(pr == "High")
            priority = High;

        QDateTime startingTime = startingTimeEdit -> dateTime();
        QDateTime endingTime = endingTimeEdit -> dateTime();
        if(startingTime > endingTime)
            throw QString("Invalid dates: ending time before starting time.");
        QDateTime whenStartToRemind = whenStartToRemindEdit -> dateTime();
        qint64 intervalOfReminding = (minutesSpinBox -> value()) * 1000 * 60 +
                (hoursSpinBox -> value()) * 1000 * 60 * 60 + (daysSpinBox -> value()) * 1000 * 60 * 60 * 24;
        if(intervalOfReminding == 0)
            throw QString("Invalid interval of reminding.");

        bool remindDuringEvent = remindDuringEventCheckBox -> isChecked();

        newEvent = new Event(summary, comment, priority, startingTime, endingTime,
                         whenStartToRemind, intervalOfReminding, remindDuringEvent);
        accept();
        close();
    }catch(QString error)
    {
        QMessageBox errorMessageBox;
        errorMessageBox.setText(error);
        errorMessageBox.exec();
    }
}

void AddNewEventDialog::cancelButtonClicked()
{
    reject();
    close();
}

Event* AddNewEventDialog::getNewEvent(bool &isNewEventHere)
{
    int code = exec();
    if(code == QDialog::Accepted)
    {
        isNewEventHere = true;
        return newEvent;
    }
    else
    {
        isNewEventHere = false;
        return new Event();
    }
}

void AddNewEventDialog::editEvent(bool &isNewEventHere, Event *event)
{
    summaryLineEdit -> insert(event -> summary());
    commentTextEdit -> setPlainText(event -> comment());

    TMPriority priority = event -> priority();
    if(priority == None)
        priorityComboBox -> setCurrentText("None");
    else if(priority == Low)
        priorityComboBox -> setCurrentText("Low");
    else if(priority == Medium)
        priorityComboBox -> setCurrentText("Medium");
    else if(priority == High)
        priorityComboBox -> setCurrentText("High");

    startingTimeEdit -> setDateTime(event -> startingTime());
    endingTimeEdit -> setDateTime(event -> endingTime());
    whenStartToRemindEdit -> setDateTime(event -> whenStartToRemind());

    daysSpinBox -> setValue(event -> intervalOfReminding() / (1000 * 60 * 60 * 24));
    hoursSpinBox -> setValue(event -> intervalOfReminding() / (1000 * 60 * 60));
    minutesSpinBox -> setValue(event -> intervalOfReminding() / (1000 * 60));

    remindDuringEventCheckBox -> setChecked(event -> remindDuringEvent());

    int code = exec();
    if(code == QDialog::Accepted)
    {
        event -> setText(newEvent -> summary());
        event -> setSummary(newEvent -> summary());
        event -> setComment(newEvent -> comment());
        event -> setPriority(newEvent -> priority());
        event -> setStartingTime(newEvent -> startingTime());
        event -> setEndingTime(newEvent -> endingTime());
        event -> setWhenStartToRemind(newEvent -> whenStartToRemind());
        event -> setIntervalOfReminding(newEvent -> intervalOfReminding());
        event -> setRemindDuringEvent(newEvent -> remindDuringEvent());
        event -> updateParameters();
        isNewEventHere = true;
    }
    else
        isNewEventHere = false;
}

AddNewEventDialog::AddNewEventDialog(QWidget *parent): QDialog(parent)
{
    setWindowTitle("Add new event");
    setModal(true);

    mainLayout = new QVBoxLayout;

    formsBox = new QGroupBox;
    formsLayout = new QFormLayout;

    summaryLineEdit = new QLineEdit;
    formsLayout -> addRow(new QLabel("Summary"), summaryLineEdit);
    commentTextEdit = new QTextEdit;
    formsLayout -> addRow(new QLabel("Сomment"), commentTextEdit);

    priorityComboBox = new QComboBox;
    priorityComboBox -> addItem("None");
    priorityComboBox -> addItem("Low");
    priorityComboBox -> addItem("Medium");
    priorityComboBox -> addItem("High");
    formsLayout -> addRow(new QLabel("Priority"), priorityComboBox);

    QDateTime currentDateTime(QDateTime::currentDateTime());
    currentDateTime.setTime(QTime(currentDateTime.time().hour(), currentDateTime.time().minute(), 0, 0));
    startingTimeEdit = new QDateTimeEdit(currentDateTime);
    startingTimeEdit -> setDisplayFormat("dd.MM.yyyy h:mm");
    formsLayout -> addRow(new QLabel("Starting time"), startingTimeEdit);
    endingTimeEdit = new QDateTimeEdit(currentDateTime);
    endingTimeEdit -> setDisplayFormat("dd.MM.yyyy h:mm");
    formsLayout -> addRow(new QLabel("Ending time"), endingTimeEdit);
    whenStartToRemindEdit = new QDateTimeEdit(currentDateTime);
    whenStartToRemindEdit -> setDisplayFormat("dd.MM.yyyy h:mm");
    formsLayout -> addRow(new QLabel("When start to remind"), whenStartToRemindEdit);

    iORLayout = new QHBoxLayout;
    daysSpinBox = new QSpinBox;
    daysSpinBox -> setRange(0, std::numeric_limits<int>::max());
    iORLayout -> addWidget(daysSpinBox);
    iORLayout -> addWidget(new QLabel(" days and "));
    hoursSpinBox = new QSpinBox;
    hoursSpinBox -> setRange(0, 23);
    iORLayout -> addWidget(hoursSpinBox);
    iORLayout -> addWidget(new QLabel(" hours and "));
    minutesSpinBox = new QSpinBox;
    minutesSpinBox -> setRange(0, 59);
    iORLayout -> addWidget(minutesSpinBox);
    iORLayout -> addWidget(new QLabel(" minutes."));
    formsLayout -> addRow(new QLabel("Repeat every "), iORLayout);


    formsBox -> setLayout(formsLayout);

    mainLayout -> addWidget(formsBox);
    remindDuringEventCheckBox = new QCheckBox("Remind during the event");
    mainLayout -> addWidget(remindDuringEventCheckBox);
    standardButtonsBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(standardButtonsBox, SIGNAL(accepted()), this, SLOT(okayButtonClicked()));
    connect(standardButtonsBox, SIGNAL(rejected()), this, SLOT(cancelButtonClicked()));
    mainLayout -> addWidget(standardButtonsBox);

    setLayout(mainLayout);
}
