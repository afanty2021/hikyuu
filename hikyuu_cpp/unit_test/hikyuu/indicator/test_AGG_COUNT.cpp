/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-09-04
 *      Author: fasiondog
 */

#include "../test_config.h"
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/plugin/interface/plugins.h>
#include <hikyuu/plugin/extind.h>
#include <hikyuu/plugin/device.h>
#include "../plugin_valid.h"

using namespace hku;

/**
 * @defgroup test_indicator_AGG_COUNT test_indicator_AGG_COUNT
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_AGG_COUNT") {
    HKU_IF_RETURN(!pluginValid(), void());

    auto k = getKData("sh000001", KQueryByDate(Datetime(20111115)));
    auto mink =
      getKData("sh000001", KQueryByDate(Datetime(20111115), Null<Datetime>(), KQuery::MIN));

    /** @arg 单日聚合分钟线 */
    auto ind = AGG_COUNT(CLOSE(), KQuery::MIN);
    auto result = ind(k);
    CHECK_EQ(result.name(), "AGG_COUNT");
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.discard(), 0);
    double sum = 0.0;
    for (const auto& v : result) {
        sum += v;
    }
    CHECK_EQ(sum, mink.size());

    /** @arg 单日聚合分钟线，复合运算 */
    auto x = (ind + ind)(k);
    sum = 0.0;
    for (const auto& v : x) {
        sum += v;
    }
    CHECK_EQ(sum, 2 * mink.size());

    /** @arg 单日聚合分钟线, 多结果集 */
    ind = AGG_COUNT(KDATA(), KQuery::MIN);
    result = ind(k);
    CHECK_EQ(result.name(), "AGG_COUNT");
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result.getResultNumber(), 6);
    sum = 0.0;
    for (const auto& v : result) {
        sum += v;
    }
    CHECK_EQ(sum, mink.size());

    /** @arg 双日滑动聚合分钟线 */
    ind = AGG_COUNT(CLOSE(), KQuery::MIN, false, 2);
    result = ind(k);
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.front(), 240);
    CHECK_EQ(result[1], 480);
    CHECK_EQ(result[15], 480);

    /** @arg 扩展K线类型 DAY3 聚合分钟线 */
    k = getKData("sh000001", KQueryByDate(Datetime(20111101), Datetime(20111117), KQuery::DAY3));
    result = AGG_COUNT(CLOSE(), KQuery::MIN)(k);
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.front(), 240);
    CHECK_EQ(result[1], 720);
    CHECK_EQ(result[2], 720);
    CHECK_EQ(result[3], 720);
    CHECK_EQ(result[4], 240);

    /** @arg 扩展K线类型 MIN3 聚合分笔线 */
    k = getKData("sh000001", KQueryByDate(Datetime(20111101), Datetime(20111103), KQuery::MIN3));
    result = AGG_COUNT(CLOSE(), KQuery::MIN)(k);
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result[0], 3);
    CHECK_EQ(result[1], 3);
}

/** @} */
