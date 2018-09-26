#pragma once

#include <fstream>

#include <QFile>
#include <QQueue>
#include <QString>

#include "state/state.h"

namespace SessionState
{
    class DownloadSingleFileState : public State
    {
    public:
        DownloadSingleFileState(::Dialog&,
                                QQueue<QString>::const_iterator,
                                quint32);
        virtual ~DownloadSingleFileState() override;
        virtual void onRead(QByteArray& buf) override;
    private:
        QQueue<QString>::const_iterator m_currentFile;
        quint32 m_fileSize;
        quint32 m_bytesWritten;
        QFile m_fstream;
    };
}
