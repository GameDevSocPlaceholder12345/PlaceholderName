/* Utility.hpp
 * ===========
 * Simple utiltiy functions clearer/easier to use
 */

#include <fstream>
#include <string>

namespace __CORE
{

    /* A replacement fstream which keeps track of the filename */
    class fstream : public std::fstream
    {
        public:
            fstream(std::string name) : _name(name), std::fstream(name) {}
            std::string name() { return this->_name; }

        private:
            std::string _name;
    };

}
