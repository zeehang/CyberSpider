#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
#include <vector>
#include <unordered_map>

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& badInteractions
                       );
    bool purge(const std::string& entity);
    
private:
    DiskMultiMap initiator;
    DiskMultiMap reverse;
    DiskMultiMap contact;
    std::string file_prefix;
    unordered_map<std::string, unsigned int> prevalence; //the object and the number of times its in it
    // Your private member declarations will go here
};

#endif // INTELWEB_H_
