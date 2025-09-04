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

using namespace hku;

static bool pluginValid() {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<ExtendIndicatorsPluginInterface>(HKU_PLUGIN_EXTEND_INDICATOR);
    return plugin && isValidLicense();
}

/**
 * @defgroup test_indicator_AGG_VAR test_indicator_AGG_VAR
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_AGG_VAR") {
    HKU_IF_RETURN(!pluginValid(), void());

    auto k = getKData("sh000001", KQueryByDate(Datetime(20111115)));
    auto mink =
      getKData("sh000001", KQueryByDate(Datetime(20111115), Null<Datetime>(), KQuery::MIN));

    /** @arg 单日分钟线聚合 */
    auto ind = AGG_VAR(CLOSE(), KQuery::MIN);
    auto result = ind(k);
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.name(), "AGG_VAR");
    CHECK_EQ(result.discard(), 0);

    auto mink2 =
      getKData("sh000001", KQueryByDate(Datetime(20111115), Datetime(20111116), KQuery::MIN));
    double sum = 0.0;
    for (auto& kr : mink2) {
        sum += kr.closePrice;
    }
    double mean = sum / mink2.size();
    sum = 0.0;
    for (auto& kr : mink2) {
        sum += std::pow(kr.closePrice - mean, 2);
    }
    mean = sum / (mink2.size() - 1);
    CHECK_EQ(result[0], doctest::Approx(mean));
}

/** @} */
