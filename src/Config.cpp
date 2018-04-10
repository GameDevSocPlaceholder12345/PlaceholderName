#include "Engine.h"

#include "rapidjson/istreamwrapper.hpp"
#include "rapidjson/error/error.hpp"
#include "rapidjson/error/en.hpp"

namespace __CORE
{

    void Engine::loadConfig(std::string confLoc)
    {
        fstream ifs(confLoc); //Should be __CORE::fstream
        rapidjson::IStreamWrapper isw(ifs);
        
        rapidjson::ParseResult pr = _config->ParseStream(isw);
        if(pr)
        {
            throw std::exception("")
        }
    }

}
