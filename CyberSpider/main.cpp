#include "DiskMultiMap.h"

int main()
{
    DiskMultiMap x;
    x.createNew("myhashtable.dat",100); // empty, with 100 buckets
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m10001");
    x.insert("blah.exe", "bletch.exe", "m0003");
    DiskMultiMap::Iterator it = x.search("hmm.exe");
    if (it.isValid())
    {
        cout << "I found at least 1 item with a key of hmm.exe\n";
        do
        {
            MultiMapTuple m = *it; // get the association
            cout << "The key is: " << m.key << endl;
            cout << "The value is: " << m.value << endl;
            cout << "The context is: " << m.context << endl;
            cout << endl;
            ++it; // advance iterator to the next matching item
        } while (it.isValid());
    }
//    DiskMultiMap x;
//    x.createNew("myhashtable.dat",100); // empty, with 100 buckets
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m10001");
//    x.insert("blah.exe", "bletch.exe", "m0003");
//    DiskMultiMap::Iterator it = x.search("goober.exe");
//    if ( ! it.isValid())
//        cout << "I couldn’t find goober.exe\n";
}