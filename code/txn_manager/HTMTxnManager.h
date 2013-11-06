#ifndef _HTM_TXN_MANAGER_H_
#define _HTM_TXN_MANAGER_H_

#include <iostream>
#include <set>

#include "TxnManager.h"       
 
class HTMTxnManager : public TxnManager {
public:
    HTMTxnManager(HashTable *table) : TxnManager(table) {}
    // TODO: Implement me
    virtual bool RunTxn(const std::vector<OpDescription> &operations,
            std::vector<string> *get_results);
private:
    unordered_map<uint64_t, atomic_flag*> lockTable;
    // Prevents concurrent insertions to the lock table.
    mutex tableMutex; 
};


#endif /* _HTM_TXN_MANAGER_H_ */