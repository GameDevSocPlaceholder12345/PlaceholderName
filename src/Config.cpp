#include "Engine.h"
#include "Utility.hpp"

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"

namespace __CORE
{

    void Engine::loadConfig(std::string confLoc)
    {
        fstream ifs(confLoc); //Should be __CORE::fstream
        rapidjson::IStreamWrapper isw(ifs);
        
        rapidjson::ParseResult pr = _config.ParseStream(isw);
        if(pr)
        {
            _Log->Error("[Engine] Failed to load config file: ", confLoc, ". Error: ", rapidjson::GetParseError_En(pr));
        }
        //HACK
        //We should have a nice custom container here, but too lazy ATM
    }

}
