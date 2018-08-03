/* Logger.cpp -> A simple logging system
 * =====================================
 * Justin Huang (MistaRhee)
 *
 * Tl;dr Logger is a simle logging system. Just start up the class and let it do it's thing. Logger
 * can be run on a separate thread (and is recommended to be) however, you can set the flag for it
 * to be a static logger.
 *
 * Licensing (Re-iterating what logger.hpp said)
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

#include "logger.hpp"


#if __unix__
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

inline void createFolder(const std::string& dirName)
{
    struct stat st = { 0 };
    if (stat(dirName.c_str(), &st) == -1)
    {
        mkdir(dirName.c_str(), 0775);
    }
}
#endif

#ifdef _WIN32
#include "dirent/dirent.h"
#include <Windows.h>

inline void createFolder(const std::string& dirName)
{
    CreateDirectory(dirName.c_str(), NULL);
}
#endif

inline bool dirExists(const std::string& dirName){
    DIR* myDir = NULL;
    myDir = opendir(dirName.c_str());
    if(myDir == NULL) return false;
    else return true;
}

namespace __logger
{

    inline std::string currentDateTime()
    {
        std::time_t result = std::time(nullptr);
        std::string rVal = std::asctime(std::localtime(&result));
        rVal.erase(rVal.end()-1);
        return rVal;
    }

    cLogger::cLogger(std::string fileLoc, loglevel_t logLevel) : logLevel(logLevel)
    {
        /* Check if there is a folder in the location -> ensure that the folder exists */
        std::string tempStr;
        this->count = 0;
        for(int i = 0; i < fileLoc.size(); i++)
        {
            if(fileLoc[i] == '/')
            {
                /* There is folder */
                if(!dirExists(tempStr))
                {
                    if(!tempStr.empty()) createFolder(tempStr);
                }
            }
            else
            {
                tempStr += fileLoc[i];
            }
        }
        tempStr.clear();

    
        this->flog.open(fileLoc, std::ofstream::out | std::ofstream::app); //defaulted to ios::out
        if(!this->flog.is_open())
        {
            std::cerr << "Critical Hit! flog failed to be opened! File: " << fileLoc << "\n";
            exit(322);
        }

#ifndef __LOGGER_NOTHREAD__
        this->dead = 0;
#endif
    }

    cLogger::~cLogger()
    {
        this->flog.flush();
        this->flog.close();
    }

    void cLogger::_log(loglevel_t l, std::string out)
    {
        if (l > this->logLevel)
        {
            return;
        }
        while(out[out.size()-1] == '\n') out.erase(out.end()-1); //Remove trailing new-lines

#ifdef __LOGGER_NOTHREAD__
        std::flog << currentDateTime() << " " << out << std::endl;
#else
        std::string output = currentDateTime() + std::string(" ") + out;
        std::unique_lock<std::mutex> ul(this->lock);
        q.Put(output);
        cv.notify_all();
#endif
    }

#ifndef __LOGGER_NOTHREAD__
    std::thread cLogger::start()
    {
        return std::thread(&cLogger::run, this);
    }

    void cLogger::setLogLevel(loglevel_t l)
    {
        this->logLevel = l;
    }

    void cLogger::run()
    {
        this->done.lock();
        while(!this->dead)
        {
            std::unique_lock<std::mutex> ul(this->lock);
            if(this->q.Empty()) cv.wait(ul);
            if(!this->q.Empty())
            { //Doing slow output so input can be fast
                do{
                    std::string out = this->q.Get();
                    this->flog << out << std::endl;
                }
                while (this->q.Size() > QUEUE_MAX);
            }
        }
        while(!this->q.Empty())
        {
            std::string out = this->q.Get();
            this->flog << out << std::endl;
        }

        this->done.unlock();
    }

    void cLogger::kill()
    {
        this->dead = 1;
        cv.notify_all();
    }

#endif
}
