#pragma once

#include "state/state.h"

namespace SessionState
{

class UpdaterInitialState : public State
{
    Q_OBJECT
public:
    UpdaterInitialState(::Protocol&);
    virtual ~UpdaterInitialState();
    virtual void onRead(QByteArray&);
};

} // namespace SessionState
