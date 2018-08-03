#include "include/boost/make_shared.hpp"
#include "include/boost/shared_ptr.hpp"

#include <iostream>

class something
{
    public:
        void dosomething()
        {
            std::cout << "Fugg" << std::endl;
        }
};

boost::shared_ptr<something> blah(){
    something s;
    return boost::make_shared<something>(s);
}

int main(){
    boost::shared_ptr<something> ptr = blah();
    ptr->dosomething();
    return 0;
}
