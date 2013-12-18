#ifndef BASETASK_H
#define BASETASK_H
#include <QString>

enum TMPriority {None, Low, Medium, High};

class BaseTask
{
public:
    BaseTask();
    BaseTask(const QString& summary, const QString& comment = "", TMPriority priority = None);

    QString summary()const;
    QString comment()const;
    TMPriority priority()const;

    void setSummary(const QString& summary);
    void setComment(const QString& comment);
    void setPriority(TMPriority priority);
protected:
    QString summary_;
    QString comment_;
    TMPriority priority_;

};

#endif // BASETASK_H
