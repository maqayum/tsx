#ifndef _TXN_MANAGER_CPP_
#define _TXN_MANAGER_CPP_

#include <iostream>
#include <set>
#include <unordered_map>
#include <utility> 

#include "TxnManager.h"

using namespace std;

bool TxnManager::ExecuteTxnOps(const vector<OpDescription> &operations,
        std::vector<string> *get_results) {
    string result;
    for (const OpDescription &op : operations) {
        if (op.type == GET && get_results != NULL) {
            ExecuteTxnOp(op, &result);
            get_results->push_back(result);
        } else {
            ExecuteTxnOp(op);
        }
    }

    return true;
}

void TxnManager::ExecuteTxnOp(const OpDescription &op, string *result) {
    if (op.type == INSERT) {
        if (result) {
            *result = (*table_)[op.key];
        }

        // Overwrite value if already present.
        (*table_)[op.key] = op.value;
    }
    else if (op.type == GET) {
        //at function throws an out_of_range exception if key not found
        const string &get_result = table_->at(op.key);
        if (result) {
            *result = get_result;
        }
    }
    else { // op.type == DELETE
        if (result) {
            *result = table_->at(op.key);
        }

        table_->erase(op.key);
    }
}

#endif /* _TXN_MANAGER_CPP */
