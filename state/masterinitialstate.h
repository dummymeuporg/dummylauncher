#pragma once

#include "state/state.h"

class Protocol;

namespace SessionState
{
    class MasterInitialState : public State
    {
    public:
        MasterInitialState(::Protocol&);
        virtual ~MasterInitialState() override;
        virtual void onRead(QByteArray& buf) override;
    };
}
