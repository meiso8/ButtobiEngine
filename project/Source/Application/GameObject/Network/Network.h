#pragma once
#include <atomic>

class Network
{
private:
    std::atomic<bool> isTicketAcquired_ = false;
public:
    void Initialize();
    void Update();
    bool GetTicketAcquired() { return isTicketAcquired_; }
};

