#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

namespace __CORE
{

    struct GenericException : std::exception
    {
    public:
        template<class... TArgs>
        GenericException(TArgs... FArgs) {
            std::ostringstream oss;
            _unpack(oss, FArgs...);
            message = oss.str();
        }
        const char* what() const throw() 
        {
            return message.c_str();
        }
        std::string message;

    private:
        void _unpack(std::ostringstream& ss) { return; } //Base case for unpacking recursion
        template<class T, class... Targs>
        void _unpack(std::ostringstream& ss, T value, Targs... Fargs)
        {
            ss << value;
            _unpack(ss, Fargs...);
        }
    };

    struct EngineException : GenericException 
    {
        template<class... TArgs>
        EngineException(TArgs... FArgs) : GenericException(FArgs...) {}
    };
    struct ConfigurationException : GenericException 
    {
        template<class... TArgs>
        ConfigurationException(TArgs... FArgs) : GenericException(FArgs...) {}
    };


}

#endif //EXCEPTION_H