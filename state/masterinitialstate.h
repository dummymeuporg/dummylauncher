#pragma once

#include "state/state.h"

namespace SessionState
{
    class MasterInitialState : public State
    {
    public:
        MasterInitialState(::Dialog&);
        virtual ~MasterInitialState() override;
        virtual void onRead(QByteArray& buf) override;
    };
}
