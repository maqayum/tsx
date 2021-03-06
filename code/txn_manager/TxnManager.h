#ifndef _TXN_MANAGER_H_
#define _TXN_MANAGER_H_

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "hashtable.h"

typedef enum OpType {
    INSERT,
    GET,
    DELETE
} OpType;

typedef struct OpDescription {
    OpType type;
    long key;
    std::string value;
} OpDescription;

typedef enum LockMode {
    READ,
    WRITE,
    FREE
} LockMode;

inline bool conflict(LockMode mode1, LockMode mode2) {
    if ((mode1 == WRITE && mode2 != FREE) ||
	(mode2 == WRITE && mode1 != FREE)) {
	return true;
    }

    return false;
}


struct ThreadStats;
class TxnManager {
public:
    // CUSTOM HASHTABLE
    TxnManager(HashTable *table) : table_(table) {}

    // UNORDERED MAP
    //TxnManager(std::unordered_map<long,std::string> *table) : table_(table) {}

    virtual ~TxnManager() {
        delete table_;
    }

    // Does whatever transaction initialization is needed, calls ExecuteTxnOps
    // to actually perform the operations, then does any necessary transaction
    // finalization/cleanup.  If get_results is non-null, it contains the
    // results of all GET operations when the function returns.
    virtual bool RunTxn(const std::vector<OpDescription> &operations,
            std::vector<std::string> *get_results,
            ThreadStats *stats) = 0;

    // Display CC stats
    virtual void printStats();

protected:
    // Returns false iff the operations were semantically ill-formed (for now,
    // this just means there were GETs or DELETEs on non-existent keys).
    virtual bool ExecuteTxnOps(const std::vector<OpDescription> &operations,
            std::vector<std::string> *get_results);

    void ExecuteTxnOp(const OpDescription &op, std::string *result=NULL);

private:
    // UNORDERED MAP
    //std::unordered_map<long, std::string> *table_;

    // CUSTOM HASHTABLE
    HashTable *table_;
};

#endif /* _TXN_MANAGER_H_ */
