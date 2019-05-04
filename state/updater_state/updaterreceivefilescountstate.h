#pragma once

#include "state/state.h"

namespace SessionState
{

class UpdaterReceiveFilesCountState : public State
{
public:
    UpdaterReceiveFilesCountState(::Protocol&);
    virtual ~UpdaterReceiveFilesCountState() override;
    virtual void onRead(QByteArray& buf) override;
};

} // namespace SessionState
