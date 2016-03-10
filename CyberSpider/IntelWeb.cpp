//
//  IntelWeb.cpp
//  CyberSpiderjustIntel
//
//  Created by Michael Zhang on 3/9/16.
//  Copyright © 2016 Michael Zhang. All rights reserved.
//
#include "IntelWeb.h"
#include <stdio.h>
IntelWeb::IntelWeb()
{
    
}

IntelWeb::~IntelWeb()
{
    //CLOSE OPEN FILES AND DELETE STUFF
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    //int loadFactor = 0.75;
    unsigned int numBuckets = maxDataItems *4 /3;
    close();
    if (!initiator.createNew(filePrefix + "initiator", numBuckets) || !reverse.createNew(filePrefix + "reverse",  numBuckets))
    {
        return false;
    }
    file_prefix = filePrefix;
    
    //  download.createNew(filePrefix+ "download", numBuckets);
    return true;
}

bool IntelWeb::openExisting(const std::string &filePrefix)
{
    close();
    if(!initiator.openExisting(filePrefix + "initiator") || !reverse.openExisting(filePrefix + "initiator"))
        return false;
    return true;
}

void IntelWeb::close()
{
    initiator.close();
    reverse.close();
}

bool IntelWeb::ingest(const std::string &telemetryFile)
{
    std::ifstream inf(telemetryFile);
    InteractionTuple toInsert;
    if (!openExisting(file_prefix))
        return false;
    std::string line;
    while(getline(inf, line))
    {
        std::string from;
        std::string to;
        std::string context;
        std::istringstream iss(line);
        if ( ! (iss >> context >> from >> to) )
        {
            std::cout << "Ignoring badly-formatted input line: " << line << std::endl;
            continue;
        }
        if(!initiator.insert(to, from, context) || !reverse.insert(from, to , context))
            return false;
    }
    return true;
}

unsigned int crawl(const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& badInteractions
                   )
{
    
    return badEntitiesFound.size();
}
