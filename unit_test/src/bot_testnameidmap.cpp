#include <iostream>
#include "bot_test.h"
#include "bot_nameidmap.h"

namespace bot {

void testNameIdMap()
{
    using namespace std;

    NameIdMap map;
    map.add("be", 0);
    map.add("aec", 1);
    map.add("abcd", 2);
    map.add("beef", 3);
    map.print();
    cout << "idx for be " << map.search("be") << endl;
    cout << "idx for abcd " << map.search("abcd") << endl;
    cout << "idx for aec " << map.search("aec") << endl;
    cout << "idx for beef " << map.search("beef") << endl;
    cout << "idx for bed " << map.search("bed") << endl;
    cout << "idx for beefo" << map.search("beefo") << endl;
    cout << "idx for f " << map.search("f") << endl;
}

} // end of namespace bot
