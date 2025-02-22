/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <queue>
#include <string>

enum UserPriority
{
    NORMAL,
    HIGH,
    EMERGENCY
}

struct UserInfo
{
    uint64_t userID;
    std::string userMessage;
    UserPriority userPriority {NORMAL};
};

queue<UserInfo> userMessageQueue;

class EventFD
{
public:
    EventFD() {}
    ~EventFD() {}

protected:
    void create(int efd)
    {
        efd = eventfd(0, 0);
        if (efd < 0)
            std::cerr << "Failed to create eventfd" << std::endl;
    }

    void sendEvent(int efd)
    {
        uint32_t value = 1;
        int ret = write(&efd, &value, sizeof(value));
        if (ret < 0)
            std::cerr << "Failed to send event" << std::endl;
    }

    void resetEvent(int efd)
    {
        uint32_t value = 0;
        int ret = read(&efd, &value, sizeof(value));
        if (ret < 0)
            std::cerr << "Failed to reset event" << std::endl;
    }

    bool waitEvent(int efd, int timeout)
    {
        // TODO: return should be int[index]
        // int pfd = poll_fd(0, 0);
        struct pollfd pfd;
        pfd[0].fd = efd;
        pfd[0].events = POLLIN;

        int ret = poll(&pfd, 0, timeout);
        if (ret <= 0)
            // no signal received
            return false;
        if (ret > 0)
        {
            if (pfd.revents && POLLIN)
            {
                return true;
            }
            return false;
        }
    }

private:
    int efd1;
};

class Thread
{
public:
    Thread(): thread_id(0) {}
    ~Thread() {}

    void start()
    {
        pthread_create(&thread_id, nullptr, threadFunction, this);
    }

    void wait()
    {
        pthread_join(thread_id, nullptr);
    }

    void stop()
    {
        std::cout << "Stop thread is called" << std::endl;
        pthread_detach(thread_id);
    }

protected:
    virtual void run() = 0;

private:
    pthread_t thread_id;

    static void* threadFunction(void* arg)
    {
        Thread* thread = static_cast<Thread*>(arg);
        thread->run();

        return nullptr;
    }
};

class FrequencyThread: public Thread, public EventFD
{
public:
    FrequencyThread(int frequency): frequency(frequency) {}
    virtual ~FrequencyThread() {};

    void run() override
    {
        create(exitSignal);
        create(dataSignal);
        eventfd efd[2] = {exitSignal, dataSignal};
        while (1)
        {
            std::cout << "attention here" << std::endl;
            // usleep((1000 / frequency) * 1000);
            // usleep(1000 * 1000);
            if (!waitEvent(efd, 1000 * 1000))
            {
                continue;
            }
            resetEvent(efd);
            UserInfo userInfo;
            userMessageQueue.pop(userInfo);
            // do command
            if (!doCommand(userInfo))
            {

            }
        }
    }

    bool doCommand(UserInfo userInfo)
    {
        switch (userInfo.userPriority)
        {

        case (NORMAL):
            break;
        case (HIGH):
            break;
        case (EMERGENCY):
            break;
        default:
            std::cerr << "Invalid user prioerity value, plase check!" << std::endl;
            return false;
        }
        // do task here
        return true;
    }

private:
    int frequency;
    int exitSignal, dataSignal;
};

class SimpleWait
{
public:
    SimpleWait()
    {
        while (1)
        {
            if (std::cin.get())
            {
                std::cout << "ENTER key is pressed, proceed to stop" << std::endl;
                break;
            }
        }
    }

    ~SimpleWait()
    {
        std::cout << "Simple Wait destructor is called" << std::endl;
    }
};

int main()
{
    std::cout<<"Hello World" << std::endl;
    FrequencyThread* thread = new FrequencyThread(5);
    thread->start();
    // thread->wait();
    SimpleWait simpleWait;
    thread->stop();
    delete thread;

    return 0;
}
