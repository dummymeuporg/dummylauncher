#pragma once

#include "state/state.h"

class Dialog;

namespace SessionState
{
    class ReceiveFilesState : public State
    {
    public:
        ReceiveFilesState(::Dialog&);
        virtual ~ReceiveFilesState() override;
        virtual void onRead(QByteArray& buf) override;
    };
}
