#include "event.h"

Event::Event(): BaseTask() {}

Event::Event(const QString& summary, const QString& comment, TMPriority priority, const QDateTime& startingTime,
             const QDateTime& endingTime, const QDateTime& whenStartToRemind,
             qint64 intervalOfReminding, bool remindDuringEvent):
    BaseTask(summary, comment, priority), QListWidgetItem(summary), startingTime_(startingTime),
    endingTime_(endingTime), whenStartToRemind_(whenStartToRemind),
    intervalOfReminding_(intervalOfReminding),
    remindDuringEvent_(remindDuringEvent), isActive_( (QDateTime::currentDateTime() <= startingTime) ||
                                                   ( remindDuringEvent &&
                                                     (QDateTime::currentDateTime() <= endingTime) ) )
{
    if(whenStartToRemind > QDateTime::currentDateTime())
        nextTimeToRemind_ = whenStartToRemind;
    else
    {
        qint64 difference = whenStartToRemind.msecsTo(QDateTime::currentDateTime());
        nextTimeToRemind_ = whenStartToRemind.addMSecs(intervalOfReminding * (difference / intervalOfReminding + 1));
    }
}

QDateTime Event::startingTime()const
{
    return startingTime_;
}

QDateTime Event::endingTime()const
{
    return endingTime_;
}

QDateTime Event::whenStartToRemind()const
{
    return whenStartToRemind_;
}

qint64 Event::intervalOfReminding()const
{
    return intervalOfReminding_;
}

QDateTime Event::nextTimeToRemind()const
{
    return nextTimeToRemind_;
}
bool Event::remindDuringEvent()const
{
    return remindDuringEvent_;
}

bool Event::isActive()const
{
    return isActive_;
}

void Event::setStartingTime(const QDateTime& startingTime)
{
    startingTime_ = startingTime;
}

void Event::setEndingTime(const QDateTime& endingTime)
{
    endingTime_ = endingTime;
}

void Event::setWhenStartToRemind(const QDateTime& whenStartToRemind)
{
    whenStartToRemind_ = whenStartToRemind;
}

void Event::setIntervalOfReminding(qint64 intervalOfReminding)
{
    intervalOfReminding_ = intervalOfReminding;
}

void Event::setRemindDuringEvent(bool remindDuringEvent)
{
    remindDuringEvent_ = remindDuringEvent;
}

void Event::moveTimeToRemind()
{
    nextTimeToRemind_ = nextTimeToRemind_.addMSecs(intervalOfReminding_);
    if( isActive_ && (nextTimeToRemind_ > startingTime_) )
        if((nextTimeToRemind_ > endingTime_) || !remindDuringEvent_)
            isActive_ = false;
}

void Event::updateParameters()
{
    if(whenStartToRemind_ > QDateTime::currentDateTime())
        nextTimeToRemind_ = whenStartToRemind_;
    else
    {
        qint64 difference = whenStartToRemind_.msecsTo(QDateTime::currentDateTime());
        nextTimeToRemind_ = whenStartToRemind_.addMSecs(intervalOfReminding_ * (difference / intervalOfReminding_ + 1));
    }

    if( (nextTimeToRemind_ > endingTime_) ||
            ((nextTimeToRemind_ > startingTime_) && !remindDuringEvent_) )
        isActive_ = false;
    else
        isActive_ = true;
}
