#pragma once

#include <QObject>

class Dialog;
class QByteArray;

namespace SessionState
{
    class State : public QObject
    {
        Q_OBJECT

    public:
        State(::Dialog&);
        virtual ~State();
        virtual void onRead(QByteArray&) = 0;
    protected:
        ::Dialog& m_dialog;
    };
}
