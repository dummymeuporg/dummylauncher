#pragma once

#include <QQueue>
#include <QString>

#include "state/state.h"

namespace SessionState
{
    class DownloadFilesState : public State
    {
    public:
        DownloadFilesState(::Dialog&, QQueue<QString>::const_iterator);
        virtual ~DownloadFilesState() override;
        virtual void onRead(QByteArray& buf) override;

    private:
        static const quint16 CODE_SEND = 1;
        static const quint16 CODE_BYE = 2;
        void _requestFile(const ::QString&);

        QQueue<QString>::const_iterator m_fileIterator;
    };
}
