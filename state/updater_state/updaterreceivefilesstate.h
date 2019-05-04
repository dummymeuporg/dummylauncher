#pragma once

#include <QtGlobal>

#include "state/state.h"

class Dialog;
class QString;

namespace SessionState
{
class UpdaterReceiveFilesState : public State
{
public:
    static const int CHUNK_SIZE = 512;
    UpdaterReceiveFilesState(::Protocol&, quint32 filecount);
    virtual ~UpdaterReceiveFilesState() override;
    virtual void onRead(QByteArray& buf) override;
private:
    static bool _hashesMatch(const QByteArray&, const QByteArray&);
    static QByteArray filesum(const QString&);
    quint32 m_filecount, m_currentFile;
};

} // namespace SessionState
