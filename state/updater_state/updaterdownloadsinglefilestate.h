#pragma once

#include <fstream>

#include <QFile>
#include <QQueue>
#include <QString>

#include "state/state.h"

namespace SessionState
{
    class UpdaterDownloadSingleFileState : public State
    {
    public:
        UpdaterDownloadSingleFileState(
            ::Protocol&,
            QQueue<QString>::const_iterator,
            quint32
        );
        virtual ~UpdaterDownloadSingleFileState() override;
        virtual void onRead(QByteArray& buf) override;
    private:
        QQueue<QString>::const_iterator m_currentFile;
        quint32 m_fileSize;
        quint32 m_bytesWritten;
        QFile m_fstream;
    };
}
