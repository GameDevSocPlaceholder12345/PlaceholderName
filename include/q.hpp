/* q.hpp -> A simple queue that supports multithreading
* =====================================================
* Justin Huang (MistaRhee)
*
* The name says it all
*
* Licensing 
* =========
* 
* MIT License
*
* Copyright (c) 2018 Justin Huang
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* ***************************************************************************************************/

#pragma once

#include <condition_variable>
#include <cstdint>
#include <exception>
#include <deque>
#include <mutex>
#include <string>

//Around just so people can do non-blocking gets.
struct QueueException : public std::exception
{
    const char* what() const throw()
    {
        return message.c_str();
    }
    QueueException(std::string in)
    {
        this->message = in;
    }
    std::string message;
};

template <class T>
class Queue 
{
    public:
        Queue() 
        {
            active = &q1;
            standby = &q2;
        }
        ~Queue() {}

        void Put(T dat)
        {
            activeSwap.lock(); //Make sure I'm not going to write to a guy which is going to change to preserve order
            standby->push_back(dat);
            cv.notify_one();
            activeSwap.unlock();
        }

        T Get(bool blocking=true) //Takes the top value, pops it
        {
            auto rVal = Front(blocking);
            active->pop_front();
            return rVal;
        }

        T Front(bool blocking=true)  //Just shows the first value, doesn't pop (so if you want to retry etc.)
        {
            /* Should only get if not empty */
            if(!blocking)
            {
                if(Empty()) throw QueueException("Tried to get on empty queue");
            }
            else 
            {
                std::unique_lock<std::mutex> ul(this->activeLock);
                if(Empty()) cv.wait(ul);
            }
            if(active->empty())
            {
                /* Swap the two guys */
                activeSwap.lock();
                auto temp = active;
                active = standby;
                standby = temp;
                activeSwap.unlock();
            }
            auto rVal = active->front();
            return rVal;
        }

        bool Empty()
        {
            return (q1.empty() && q2.empty());
        }

        uint32_t Size(){
            return (q1.size() + q2.size());
        }



    private:
        std::deque<T>* active;
        std::deque<T>* standby;
        
        std::deque<T> q1;
        std::deque<T> q2;

        std::mutex activeSwap;

        /* Blocking until there is data to be gotten */
        std::mutex activeLock;
        std::condition_variable cv;
};
