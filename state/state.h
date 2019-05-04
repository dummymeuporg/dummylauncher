#pragma once

#include <QObject>

class Protocol;
class QByteArray;

namespace SessionState
{
    class State : public QObject
    {
        Q_OBJECT

    public:
        State(::Protocol&);
        virtual ~State();
        virtual void onRead(QByteArray&) = 0;
    protected:
        ::Protocol& m_protocol;
    };
}
