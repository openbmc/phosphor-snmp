#include <iostream>
#include "snmp.hpp"

int main(int argc,char** argv)
{
    using namespace phosphor::snmp;
    sendTrap<ErrorNotification>(1,23456,2,"ErrorTest",1);
}
