/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-05-30
 *      Author: fasiondog
 */

#include "hikyuu/plugin/interface/plugins.h"
#include "hikyuu/plugin/device.h"
#include "Performance.h"
#include "TradeManagerBase.h"

namespace hku {

Performance TradeManagerBase::getPerformance(const Datetime& datetime, const KQuery::KType& ktype) {
    Performance ret;
    if (isValidLicense()) {
        auto& sm = StockManager::instance();
        auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
        HKU_ERROR_IF_RETURN(!plugin, ret, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
        ret = plugin->getExtPerformance(shared_from_this(), datetime, ktype);
    } else {
        ret = Performance();
        ret.statistics(shared_from_this(), datetime);
    }

    return ret;
}

price_t TradeManagerBase::getMaxPullBack(const Datetime& date, const KQuery::KType& ktype) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
    HKU_ERROR_IF_RETURN(!plugin, 0.0, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
    return plugin->getMaxPullBack(shared_from_this(), date, ktype);
}

std::vector<PositionExtInfo> TradeManagerBase::TradeManagerBase::getHistoryPositionExtInfoList(
  const KQuery::KType& ktype, int trade_mode) {
    std::vector<PositionExtInfo> ret;
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
    HKU_ERROR_IF_RETURN(!plugin, ret, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
    ret = plugin->getHistoryPositionExtInfoList(shared_from_this(), ktype, trade_mode);
    return ret;
}

std::vector<PositionExtInfo> TradeManagerBase::getPositionExtInfoList(const Datetime& current_time,
                                                                      const KQuery::KType& ktype,
                                                                      int trade_mode) {
    std::vector<PositionExtInfo> ret;
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
    HKU_ERROR_IF_RETURN(!plugin, ret, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
    ret = plugin->getPositionExtInfoList(shared_from_this(), current_time, ktype, trade_mode);
    return ret;
}

std::vector<std::pair<Datetime, double>> TradeManagerBase::getProfitPercentMonthly(
  const Datetime& datetime) {
    std::vector<std::pair<Datetime, double>> ret;
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
    HKU_ERROR_IF_RETURN(!plugin, ret, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
    ret = plugin->getProfitPercentMonthly(shared_from_this(), datetime);
    return ret;
}

std::vector<std::pair<Datetime, double>> TradeManagerBase::getProfitPercentYearly(
  const Datetime& datetime) {
    std::vector<std::pair<Datetime, double>> ret;
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<TMReportPluginInterface>(HKU_PLUGIN_TMREPORT);
    HKU_ERROR_IF_RETURN(!plugin, ret, "Can't find {} plugin!", HKU_PLUGIN_TMREPORT);
    ret = plugin->getProfitPercentYearly(shared_from_this(), datetime);
    return ret;
}

}  // namespace hku