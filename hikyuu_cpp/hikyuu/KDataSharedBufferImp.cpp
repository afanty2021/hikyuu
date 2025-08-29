/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-08-28
 *      Author: fasiondog
 */

#include <functional>
#include "StockManager.h"
#include "KDataSharedBufferImp.h"

namespace hku {

KDataSharedBufferImp::KDataSharedBufferImp(const Stock& stock, const KQuery& query)
: KDataImp(stock, query) {
    std::shared_lock<std::shared_mutex> lock(*(m_stock.m_data->pMutex[m_query.kType()]));
    m_data = m_stock.m_data->pKData[m_query.kType()]->data() + m_start;
}

KDataSharedBufferImp::~KDataSharedBufferImp() {}

size_t KDataSharedBufferImp::getPos(const Datetime& datetime) const {
    KRecord null_record;
    if (empty()) {
        return Null<size_t>();
    }

    size_t mid, low = 0, high = size() - 1;
    while (low <= high) {
        if (datetime > getKRecord(high).datetime) {
            mid = high + 1;
            break;
        }

        if (getKRecord(low).datetime >= datetime) {
            mid = low;
            break;
        }

        mid = (low + high) / 2;
        if (datetime > getKRecord(mid).datetime) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (mid >= size()) {
        return Null<size_t>();
    }

    return getKRecord(mid).datetime == datetime ? mid : Null<size_t>();
}

const KRecord& KDataSharedBufferImp::getKRecord(size_t pos) const {
    return pos < m_size ? m_data[pos] : KRecord::NullKRecord;
}

DatetimeList KDataSharedBufferImp::getDatetimeList() const {
    DatetimeList result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i].datetime;
    }
    return result;
}

} /* namespace hku */
