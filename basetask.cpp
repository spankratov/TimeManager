#include "basetask.h"

BaseTask::BaseTask() {}

BaseTask::BaseTask(const QString &summary, const QString &comment, TMPriority priority):
    summary_(summary), comment_(comment), priority_(priority) {}

QString BaseTask::summary()const
{
    return summary_;
}

QString BaseTask::comment()const
{
    return comment_;
}

TMPriority BaseTask::priority()const
{
    return priority_;
}

void BaseTask::setSummary(const QString& summary)
{
    summary_ = summary;
}

void BaseTask::setComment(const QString &comment)
{
    comment_ = comment;
}

void BaseTask::setPriority(TMPriority priority)
{
    priority_ = priority;
}
