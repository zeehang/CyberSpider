#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include <functional>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator()
        {
            currentLoc = 0;
        }
        
        Iterator(BinaryFile::Offset bucketLoc, DiskMultiMap* mapptr)
        {
            currentLoc = bucketLoc;
            m_mapptr = mapptr;
        }
        // You may add additional constructors
        bool isValid() const
        {
            if(currentLoc !=0)
                return true;
            else
                return false;
        }
        Iterator& operator++()
        {
            if (isValid() == false)
                return *this;
            DiskNode test;
            m_mapptr -> tracker.read(test, currentLoc);
            if (test.next == 0)
                currentLoc = 0;
            else
                currentLoc = test.next;
            return *this;
        }
        MultiMapTuple operator*()
        {
            MultiMapTuple returnTuple;
            if (isValid() == false)
                return returnTuple;
            DiskNode test;
            m_mapptr -> tracker.read(test, currentLoc);
            returnTuple.key = test.key;
            returnTuple.value = test.value;
            returnTuple.context = test.context;
            return returnTuple;
        }
        
    private:
        BinaryFile::Offset currentLoc;
        DiskMultiMap* m_mapptr;
        // Your private member declarations will go here
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    const int MAX_NUMBER_CHARS=120;
    BinaryFile tracker;
    int numKeys;
    struct DiskNode
    {
        char key[120 +1];
        char value[120 +1];
        char context[120+1];
        BinaryFile::Offset next;
    };
    struct Header
    {
        BinaryFile::Offset head;     // head of the list of nodes being used
        BinaryFile::Offset freeList; // head of the list of nodes available for reuse
        
    };
    
    BinaryFile::Offset hashFunction (const std::string& input);
    
};

#endif // DISKMULTIMAP_H_