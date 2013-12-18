#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <QListWidgetItem>

#include "basetask.h"

class Event: public BaseTask, public QListWidgetItem
// Event has starting time and ending time. The program will remind the user about an event
// in the way he wants it
{
public:
    Event();
    Event(const QString& summary, const QString& comment, TMPriority priority, const QDateTime& startingTime,
          const QDateTime& endingTime, const QDateTime& whenStartToRemind,
          qint64 intervalOfReminding, bool remindDuringEvent = true);

    QDateTime startingTime()const;
    QDateTime endingTime()const;
    QDateTime whenStartToRemind()const;
    qint64 intervalOfReminding()const; // in milliseconds
    QDateTime nextTimeToRemind()const;
    bool remindDuringEvent()const;
    bool isActive()const;

    void setStartingTime(const QDateTime& startingTime);
    void setEndingTime(const QDateTime& endingTime);
    void setWhenStartToRemind(const QDateTime& whenStartToRemind);
    void setIntervalOfReminding(qint64 intervalOfReminding);
    void setRemindDuringEvent(bool remindDuringEvent);

    void moveTimeToRemind();
    void updateParameters();
private:
    QDateTime startingTime_;
    QDateTime endingTime_;
    QDateTime whenStartToRemind_;
    qint64 intervalOfReminding_; // in milliseconds
    QDateTime nextTimeToRemind_;
    bool remindDuringEvent_;
    bool isActive_;
};

#endif // EVENT_H
