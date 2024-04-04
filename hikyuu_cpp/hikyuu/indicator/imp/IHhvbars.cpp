/*
 * IHhvbars.cpp
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-4-11
 *      Author: fasiondog
 */

#include "IHhvbars.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::IHhvbars)
#endif

namespace hku {

IHhvbars::IHhvbars() : IndicatorImp("HHVBARS", 1) {
    setParam<int>("n", 20);
}

IHhvbars::~IHhvbars() {}

void IHhvbars::_checkParam(const string& name) const {
    if ("n" == name) {
        HKU_ASSERT(getParam<int>("n") >= 0);
    }
}

void IHhvbars::_calculate(const Indicator& ind) {
    size_t total = ind.size();
    if (0 == total) {
        m_discard = 0;
        return;
    }

    if (ind.discard() >= total) {
        m_discard = total;
        return;
    }

    m_discard = ind.discard();
    if (1 == total) {
        if (0 == m_discard) {
            _set(0, 0);
        }
        return;
    }

    int n = getParam<int>("n");
    if (0 == n) {
        n = total - m_discard;
    } else if (n > total) {
        n = total;
    }

    auto const* src = ind.data();
    auto* dst = this->data();

    price_t max = src[m_discard];
    size_t pre_pos = m_discard;
    size_t start_pos = m_discard + n < total ? m_discard + n : total;
    for (size_t i = m_discard; i < start_pos; i++) {
        if (src[i] >= max) {
            max = src[i];
            pre_pos = i;
        }
        dst[i] = i - pre_pos;
    }

    for (size_t i = start_pos; i < total; i++) {
        size_t j = i + 1 - n;
        if (pre_pos < j) {
            pre_pos = j;
            max = src[j];
            for (size_t k = pre_pos + 1; k <= i; k++) {
                if (src[k] >= max) {
                    max = src[k];
                    pre_pos = k;
                }
            }

        } else {
            if (src[i] >= max) {
                max = src[i];
                pre_pos = i;
            }
        }
        dst[i] = i - pre_pos;
    }
}

void IHhvbars::_dyn_run_one_step(const Indicator& ind, size_t curPos, size_t step) {
    size_t start = _get_step_start(curPos, step, ind.discard());
    price_t maxVal = ind[start];
    size_t maxPos = start;
    for (size_t i = start + 1; i <= curPos; i++) {
        if (ind[i] > maxVal) {
            maxVal = ind[i];
            maxPos = i;
        }
    }
    _set(curPos - maxPos, curPos);
}

Indicator HKU_API HHVBARS(int n) {
    IndicatorImpPtr p = make_shared<IHhvbars>();
    p->setParam<int>("n", n);
    return Indicator(p);
}

Indicator HKU_API HHVBARS(const IndParam& n) {
    IndicatorImpPtr p = make_shared<IHhvbars>();
    p->setIndParam("n", n);
    return Indicator(p);
}

} /* namespace hku */
