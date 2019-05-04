#include "updaterinitalstate.h"

#include "protocol/protocol.h"

#include "state/updater_state/updaterinitalstate.h"
#include "state/updater_state/updaterreceivefilescountstate.h"

namespace SessionState
{

UpdaterInitialState::UpdaterInitialState(::Protocol& protocol)
    : State(protocol)
{

}

UpdaterInitialState::~UpdaterInitialState()
{

}

void UpdaterInitialState::onRead(QByteArray& buf)
{
    QString data(buf);
    qDebug() << "Initial state: " << buf.data();
    if (data == "OK")
    {
        qDebug() << "Switch to receive files state.";
        m_protocol.setState(
                    new UpdaterReceiveFilesCountState(m_protocol)
        );
    }
}


} // namespace SessionState
