#pragma once

#include "state/state.h"

namespace SessionState
{

    class ReceiveFilesCountState : public State
    {
    public:
        ReceiveFilesCountState(::Dialog&);
        virtual ~ReceiveFilesCountState() override;
        virtual void onRead(QByteArray& buf) override;
    };
}
