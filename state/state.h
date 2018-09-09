#pragma once

class Dialog;

namespace SessionState
{
    class State
    {
    public:
        State(::Dialog&);
    protected:
        ::Dialog& m_dialog;
    };
}
