/* Logger.hpp -> A simple logging system
 * =====================================
 * Justin Huang (MistaRhee)
 *
 * Tl;dr Logger is a simle logging system. Just start up the class and let it do it's thing. Logger
 * can be run on a separate thread (and is recommended to be) however, you can set the flag for it
 * to be a static logger.
 *
 * Licensing (Really? You power hungry people -_-)
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

#ifndef LOGGER_HPP
#define LOGGER_HPP

//Uncomment the following line for no threading (why?)
//#define __LOGGER_NOTHREAD__
#ifndef __LOGGER_NOTHREAD__
#include <condition_variable>
#include <mutex>
#include <thread>

#define QUEUE_MAX 1000 //Specify the largest size the queue can be before the logger blocks and forces output (to prevent this from eating up large chunks of memory)

#endif

#include <cstdio> //Yes, I'm an old boy. Deal with it
#include <ctime>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//My own queue that does handles multithreaded access
#include "q.hpp"

//JH (27/03/2018): Added boost_ptr exporting cLogger shared_ptr for "better" practises
#include "boost/shared_ptr.hpp"
typedef boost::shared_ptr<__logger::cLogger> logger_ptr;

namespace __logger{

    enum loglevel_t{
        CRIT,
        ERR,
        WARN,
        INFO,
        DEBUG
    };

    class cLogger{
        public:
            cLogger() : cLogger("wooden.log") {}
            cLogger(std::string fout) : cLogger(fout, ERR) {} //Logs to the specified file
            cLogger(std::string, loglevel_t);
            ~cLogger();
            
            template<class... TArgs>
            void crit(TArgs... Fargs) { log(CRIT, Fargs...); }
            template<class... TArgs>
            void err(TArgs... Fargs) { log(ERR, Fargs...); }
            template<class... TArgs>
            void warn(TArgs... Fargs) { log(WARN, Fargs...); }
            template<class... TArgs>
            void info(TArgs... Fargs) { log(INFO, Fargs...); }
            template<class... TArgs>
            void debug(TArgs... Fargs) { log(DEBUG, Fargs...); }

            /* Concactenates all the arguments (no sep) */
            template<class... TArgs>
            void log(loglevel_t l, TArgs... Fargs)
            {
                /* Concact the string, then add it to the log queue */
                std::ostringstream out;
                __unpack(out, Fargs...);
                _log(l, out.str());
            }

            void setLogLevel(loglevel_t);
#ifndef __LOGGER_NOTHREAD__
            std::thread start();
            void run();
            void kill();

            std::mutex done; //A blocking mutex to ensure that all things have been logged before deleting the object (usually used if the main function has a deconstructor or something(
            std::condition_variable cv;
#endif

        private:
#ifndef __LOGGER_NOTHREAD__
            Queue<std::string> q;
            bool dead;
            std::mutex lock;
#endif
            void _log(loglevel_t level, std::string);

            unsigned int count;
            std::ofstream flog;

            loglevel_t logLevel;

            /* Unpack functions. Yey */
            void __unpack(std::ostringstream& ss) { return; } //Base case for unpacking recursion
            template<class T, class... Targs>
            void __unpack(std::ostringstream& ss, T value, Targs... Fargs)
            {
                ss << value;
                __unpack(ss, Fargs...);
            }

    };
}


#endif //LOGGER_HPP
