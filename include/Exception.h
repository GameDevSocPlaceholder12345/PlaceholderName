#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace __CORE
{

    class GenericException : std::exception
    {
        const char* what() const throw() 
        {
            return message.c_str();
        }
        
        GenericException(std::string in) message(in) {}
        std::string message;
    };

    class EngineException : public GenericException 
    {
        EngineException(std::string in) : GenericException(in) {}
    };
    class ConfigurationException : public GenericException {
        ConfigurationException(std::string in) : GenericException(in) {}
    };

}

#endif //EXCEPTION_H