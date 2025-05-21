/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-05-19
 *      Author: fasiondog
 */

#pragma once

#include "hikyuu/indicator/Indicator.h"

namespace hku {

/**
 * @brief 将指标数据转换到指定周期
 * @param ktype 指定周期
 * @param fill_null 扩展时填充null数据，否则为使用最近值填充
 * @return Indicator
 */
Indicator HKU_API WITHKTYPE(const KQuery::KType& ktype, bool fill_null = false);

Indicator HKU_API WITHKTYPE(const Indicator& ind, const KQuery::KType& ktype,
                            bool fill_null = false);

inline Indicator WITHDAY(bool fill_null = false) {
    return WITHKTYPE(KQuery::DAY, fill_null);
}

inline Indicator WITHDAY(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::DAY, fill_null);
}

inline Indicator WITHWEEK(bool fill_null = false) {
    return WITHKTYPE(KQuery::WEEK, fill_null);
}

inline Indicator WITHWEEK(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::WEEK, fill_null);
}

inline Indicator WITHMONTH(bool fill_null = false) {
    return WITHKTYPE(KQuery::MONTH, fill_null);
}

inline Indicator WITHMONTH(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MONTH, fill_null);
}

inline Indicator WITHQUARTER(bool fill_null = false) {
    return WITHKTYPE(KQuery::QUARTER, fill_null);
}

inline Indicator WITHQUARTER(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::QUARTER, fill_null);
}

inline Indicator WITHHALFYEAR(bool fill_null = false) {
    return WITHKTYPE(KQuery::HALFYEAR, fill_null);
}

inline Indicator WITHHALFYEAR(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::HALFYEAR, fill_null);
}

inline Indicator WITHYEAR(bool fill_null = false) {
    return WITHKTYPE(KQuery::YEAR, fill_null);
}

inline Indicator WITHYEAR(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::YEAR, fill_null);
}

inline Indicator WITHMIN(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN, fill_null);
}

inline Indicator WITHMIN(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN, fill_null);
}

inline Indicator WITHMIN5(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN5, fill_null);
}

inline Indicator WITHMIN5(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN5, fill_null);
}

inline Indicator WITHMIN15(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN15, fill_null);
}

inline Indicator WITHMIN15(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN15, fill_null);
}

inline Indicator WITHMIN30(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN30, fill_null);
}

inline Indicator WITHMIN30(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN30, fill_null);
}

inline Indicator WITHMIN60(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN60, fill_null);
}

inline Indicator WITHMIN60(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN60, fill_null);
}

inline Indicator WITHHOUR(bool fill_null = false) {
    return WITHKTYPE(KQuery::MIN60, fill_null);
}

inline Indicator WITHHOUR(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::MIN60, fill_null);
}

inline Indicator WITHHOUR2(bool fill_null = false) {
    return WITHKTYPE(KQuery::HOUR2, fill_null);
}

inline Indicator WITHHOUR2(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::HOUR2, fill_null);
}

inline Indicator WITHHOUR4(bool fill_null = false) {
    return WITHKTYPE(KQuery::HOUR4, fill_null);
}

inline Indicator WITHHOUR4(const Indicator& ind, bool fill_null = false) {
    return WITHKTYPE(ind, KQuery::HOUR4, fill_null);
}

}  // namespace hku