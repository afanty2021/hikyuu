/*
 *  Copyright (c) 2024 hikyuu.org
 *
 *  Created on: 2024-12-20
 *      Author: fasiondog
 */

#include <ta-lib/ta_func.h>
#include "TaMacd.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::TaMacd)
#endif

namespace hku {

TaMacd::TaMacd() : IndicatorImp("TA_MACD", 3) {
    setParam<int>("fast_n", 12);
    setParam<int>("slow_n", 26);
    setParam<int>("signal_n", 9);
}

void TaMacd::_checkParam(const string& name) const {
    if (name == "fast_n" || name == "slow_n") {
        int n = getParam<int>(name);
        HKU_CHECK(n >= 2 && n <= 100000, "{} must be in [2, 100000]!", name);
    } else if (name == "signal_n") {
        int signal_n = getParam<int>("signal_n");
        HKU_ASSERT(signal_n >= 1 && signal_n <= 100000);
    }
}

void TaMacd::_calculate(const Indicator& data) {
    int fast_n = getParam<int>("fast_n");
    int slow_n = getParam<int>("slow_n");
    int signal_n = getParam<int>("signal_n");
    size_t total = data.size();
    int lookback = TA_MACD_Lookback(fast_n, slow_n, signal_n);
    if (lookback < 0) {
        m_discard = total;
        return;
    }

    m_discard = data.discard() + lookback;
    if (m_discard >= total) {
        m_discard = total;
        return;
    }

    const double* src = data.data();
    auto* dst0 = this->data(0);
    auto* dst1 = this->data(1);
    auto* dst2 = this->data(2);

    int outBegIdx;
    int outNbElement;
    TA_MACD(m_discard, total - 1, src, fast_n, slow_n, signal_n, &outBegIdx, &outNbElement,
            dst0 + m_discard, dst1 + m_discard, dst2 + m_discard);
    HKU_ASSERT(outBegIdx == m_discard && (outBegIdx + outNbElement) <= total);
}

Indicator HKU_API TA_MACD(int fast_n, int slow_n, int signal_n) {
    auto p = make_shared<TaMacd>();
    p->setParam<int>("fast_n", fast_n);
    p->setParam<int>("slow_n", slow_n);
    p->setParam<int>("signal_n", signal_n);
    return Indicator(p);
}

} /* namespace hku */
