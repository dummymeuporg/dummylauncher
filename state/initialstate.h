#pragma once

#include "state/state.h"

class Dialog;

namespace SessionState
{
    class InitialState : public State
    {
    public:
        InitialState(::Dialog&);
        virtual ~InitialState() override;
        virtual void onRead(QByteArray& buf) override;
    };
}
