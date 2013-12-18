#ifndef ADD_NEW_EVENT_DIALOG_H
#define ADD_NEW_EVENT_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QTextEdit>
#include <QGroupBox>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <limits>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QMessageBox>

#include "event.h"

class AddNewEventDialog: public QDialog
{
    Q_OBJECT
public:
    AddNewEventDialog(QWidget* parent = 0);
    Event* getNewEvent(bool& isNewEventHere);
    void editEvent(bool& isNewEventHere, Event* event);
private slots:
    void okayButtonClicked();
    void cancelButtonClicked();
private:
    QVBoxLayout* mainLayout;
    QGroupBox* formsBox;
    QFormLayout* formsLayout;
    QLineEdit* summaryLineEdit;
    QTextEdit* commentTextEdit;
    QComboBox* priorityComboBox;
    QDateTimeEdit* startingTimeEdit;
    QDateTimeEdit* endingTimeEdit;
    QDateTimeEdit* whenStartToRemindEdit;
    QHBoxLayout* iORLayout;
    QSpinBox* daysSpinBox;
    QSpinBox* hoursSpinBox;
    QSpinBox* minutesSpinBox;
    QCheckBox* remindDuringEventCheckBox;
    QDialogButtonBox* standardButtonsBox;

    Event* newEvent;
};

#endif // ADD_NEW_EVENT_DIALOG_H
