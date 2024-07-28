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

class FrequencyThread: public Thread
{
public:
    FrequencyThread(int frequency): frequency(frequency) {}
virtual ~FrequencyThread() {};

    void run() override
    {
        while (1)
        {
            std::cout << "attention here" << std::endl;
            // usleep((1000 / frequency) * 1000);
            usleep(1000 * 1000);
        }
    }
private:
    int frequency;
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