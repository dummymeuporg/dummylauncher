#pragma once

#include <QtGlobal>

#include "state/state.h"

class Dialog;
class QString;

namespace SessionState
{
    class ReceiveFilesState : public State
    {
    public:
        static const int CHUNK_SIZE = 512;
        ReceiveFilesState(::Dialog&, quint32 filecount);
        virtual ~ReceiveFilesState() override;
        virtual void onRead(QByteArray& buf) override;
    private:
        static QByteArray filesum(const QString&);
        quint32 m_filecount, m_currentFile;
    };
}
