#include "protocol/protocol.h"

#include "state/updater_state/updaterreceivefilescountstate.h"
#include "state/updater_state/updaterreceivefilesstate.h"

namespace SessionState
{

UpdaterReceiveFilesCountState::UpdaterReceiveFilesCountState(
    ::Protocol& protocol) : State(protocol)
{

}

UpdaterReceiveFilesCountState::~UpdaterReceiveFilesCountState()
{

}

void UpdaterReceiveFilesCountState::onRead(QByteArray& buf)
{
    QDataStream in(buf);
    in.setByteOrder(QDataStream::LittleEndian);
    quint32 filecount;

    in >> filecount;
    //m_dialog.setStatus(tr("Number of files: %n", "", int(filecount)));
    m_protocol.setState(
        new UpdaterReceiveFilesState(m_protocol, filecount));
}

} // namespace SessionState
