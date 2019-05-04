#pragma once

#include "protocol/protocol.h"

class Dialog;

class UpdaterProtocol : public Protocol
{
public:
    UpdaterProtocol(Dialog&);
public slots:
    virtual void onConnect() override;
    virtual void onDisconnect() override;
    void addDownload(const QString&);
    const QQueue<QString>& downloadList() const
    {
        return m_downloadList;
    }

    void updateDownloadProgress();
private:
    Dialog& m_dialog;
    QQueue<QString> m_downloadList;

};
