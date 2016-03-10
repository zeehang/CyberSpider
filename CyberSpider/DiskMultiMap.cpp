

//
//  DiskMultiMap.cpp
//  CyberSpider
//
//  Created by Michael Zhang on 3/8/16.
//  Copyright © 2016 Michael Zhang. All rights reserved.
//

#include "DiskMultiMap.h"

DiskMultiMap::DiskMultiMap()
{
    
}

DiskMultiMap::~DiskMultiMap()
{
    
}

bool DiskMultiMap::createNew(const std::string &filename, unsigned int numBuckets)
{
    close();
    if(tracker.createNew(filename) == false)
        return false;
    numKeys = numBuckets;
    Header h;
    h.head = sizeof(Header);
    h.freeList = 0;
    if (tracker.write(h, 0) == false)
        return false;
    BinaryFile::Offset bucketInit = 0;
    for (int i=0; i< numBuckets; i++)
    {
        if(tracker.write(bucketInit, tracker.fileLength())== false)
            return false;
    }
    
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
    close();
    return tracker.openExisting(filename);
}

void DiskMultiMap::close()
{
    tracker.close();
    return;
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{

    if(key.length() > 120 || value.length() >120 || context.length() > 120)
        return false;
    BinaryFile::Offset keyToInsert = hashFunction(key);
    
    BinaryFile::Offset addressWhereNodePointsTO;
    DiskNode toInsert;
    strcpy(toInsert.key, key.c_str());
    strcpy(toInsert.value, value.c_str());
    strcpy(toInsert.context, context.c_str());
    BinaryFile::Offset none = 0;
    toInsert.next = none;
    tracker.read(addressWhereNodePointsTO, sizeof(Header)+ keyToInsert);
    BinaryFile::Offset locOfNewNode = acquireNode();
    std::cerr << "Adress where node points to; "<< addressWhereNodePointsTO << std::endl;
    if(addressWhereNodePointsTO == 0)
    {
        tracker.write(locOfNewNode, sizeof(Header)+ keyToInsert);
        tracker.write(toInsert, locOfNewNode);
        
    }
    else{
        DiskNode iter;
        tracker.write(toInsert, locOfNewNode);
        tracker.read(iter, addressWhereNodePointsTO);
        while (iter.next != 0)
        {
            addressWhereNodePointsTO = iter.next;
            tracker.read(iter, addressWhereNodePointsTO);
        }
        iter.next = locOfNewNode;
        tracker.write(iter, addressWhereNodePointsTO);
        //CHANGE THIS SO toinsert.next points to wear the binary offset of the bucket used to be - then apply this philosophy to erase
        
    }
    std::cerr << "Inserting new node at location: " << locOfNewNode << std::endl;
    return true;

}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    
    BinaryFile::Offset bucketLocation = hashFunction(key);
    BinaryFile::Offset temp;
    tracker.read(temp, bucketLocation + sizeof(Header));
    if (temp == 0)
    {
        Iterator toReturn;
        return toReturn;
    }
    else{
        Iterator toReturn(temp, this);
        return toReturn;
    }
}

BinaryFile::Offset DiskMultiMap::hashFunction (const std::string& input)
{
    hash<std::string> hasher;
    unsigned int newHash = hasher(input);
    BinaryFile::Offset bucketLocation = newHash % numKeys;
    return bucketLocation;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    int nodesDeleted = 0;
    DiskNode checkValues;
    BinaryFile::Offset bucketLoc = hashFunction(key);
    BinaryFile::Offset locofNode;
    BinaryFile::Offset none = 0;
    tracker.read(locofNode, bucketLoc + sizeof(Header));
    if (locofNode == 0)
        return 0;

    else{
        BinaryFile::Offset current;
        BinaryFile::Offset previous;
        BinaryFile::Offset oldNext;
        DiskNode iter;
        
        Header h;
        tracker.read(h, 0);
        current = locofNode;
        tracker.read(iter, current);
        previous = 0;
        do{
            tracker.read(iter, current);
            tracker.read(h,0);
            oldNext = iter.next;//why does this not return 0?
            if (iter.key == key && iter.value == value && iter.context == context)
            {

                    std::cerr << "Erasing node at location: " << current << std::endl;
                    if (previous == 0) //the first one after the bucket
                    {
                        BinaryFile::Offset newLocofNode = iter.next;
                        tracker.write(newLocofNode, bucketLoc + sizeof(Header));
                    }
                    else
                    {
                    DiskNode previousNewLink;
                    tracker.read(previousNewLink, previous);
                    previousNewLink.next = iter.next;
                         tracker.write(previousNewLink, previous);
                    }
                    //next pointer on the deleted node??
                    if (h.freeList == 0)
                    {
                       
                        h.freeList = current;
                        iter.next = none;
                        tracker.write(iter, current);
                        tracker.write(h,0);
                    }
                    else{
                        iter.next = h.freeList;
                        h.freeList = current;
                        tracker.write(iter, current);
                        tracker.write(h,0);
                        }
                    if (previous !=0)
                        previous = current;
                    current = oldNext;
                              nodesDeleted++;
                
            }
            else{
            
            previous = current;
            current = oldNext;
            }
            
        }while(current != 0);

    }
    return nodesDeleted;
}

BinaryFile::Offset DiskMultiMap::acquireNode()
{
    Header h;
    tracker.read(h, 0);
    if (h.freeList == 0)
        return tracker.fileLength();
    else
    {
        BinaryFile::Offset toReturn = h.freeList;
        DiskNode iter;
        tracker.read(iter,h.freeList);
        h.freeList = iter.next;
        tracker.write(h, 0);
        return toReturn;
    }
}
