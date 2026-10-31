#pragma once
#include <atomic>

class Network
{
private:

     std::atomic<bool> isTicketGet_ = false;
public:
    Network();
    
    ~Network();

    bool GetIsTicketGet() {
        return isTicketGet_;
    }
    void Initialize();
    void OnButtonClicked();
};
