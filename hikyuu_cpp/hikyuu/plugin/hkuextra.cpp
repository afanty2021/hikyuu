/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-08-06
 *      Author: fasiondog
 */

#include "hikyuu/KDataPrivatedBufferImp.h"
#include "interface/plugins.h"
#include "hkuextra.h"

namespace hku {

void HKU_API registerExtraKType(const string& ktype, const string& basetype, int32_t minutes,
                                std::function<Datetime(const Datetime&)> getPhaseEnd) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, void(), "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    plugin->registerKTypeExtra(ktype, basetype, minutes, getPhaseEnd);
}

void HKU_API registerExtraKType(const string& ktype, const string& basetype, int32_t nbars) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, void(), "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    plugin->registerKTypeExtra(ktype, basetype, nbars, std::function<Datetime(const Datetime&)>());
}

void HKU_API releaseExtraKType() {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, void());
    plugin->releaseKExtra();
}

bool isExtraKType(const string& ktype) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, false);
    return plugin->isExtraKType(ktype);
}

int32_t getKTypeExtraMinutes(const string& ktype) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, 0, "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    return plugin->getKTypeExtraMinutes(ktype);
}

std::vector<string> getExtraKTypeList() {
    std::vector<string> ret;
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, ret);
    ret = plugin->getExtraKTypeList();
    return ret;
}

KRecordList getExtraKRecordList(const Stock& stk, const KQuery& query) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, KRecordList());
    return plugin->getExtraKRecordList(stk, query);
}

size_t getStockExtraCount(const Stock& stk, const string& ktype) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, 0);
    return plugin->getStockExtraCount(stk, ktype);
}

KDataImpPtr getKDataImp(const Stock& stk, const KQuery& query) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, make_shared<KDataPrivatedBufferImp>(stk, query));
    return plugin->getKDataImp(stk, query);
}

bool getStockExtraIndexRange(const Stock& stk, const KQuery& query, size_t& out_start,
                             size_t& out_end) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_IF_RETURN(!plugin, false);
    return plugin->getStockExtraIndexRange(stk, query, out_start, out_end);
}

//-------------------------------
// Views 扩展
//-------------------------------

std::shared_ptr<arrow::Table> getIndicatorsViewParallel(const StockList& stks,
                                                        const IndicatorList& inds,
                                                        const KQuery& query, const string& market) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, nullptr, "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    return plugin->getIndicatorsViewParallel(stks, inds, query, market);
}

std::shared_ptr<arrow::Table> getIndicatorsViewParallel(const StockList& stks,
                                                        const IndicatorList& inds,
                                                        const Datetime& date, size_t cal_len,
                                                        const KQuery::KType& ktype,
                                                        const string& market) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, nullptr, "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    return plugin->getIndicatorsViewParallel(stks, inds, date, cal_len, ktype, market);
}

std::shared_ptr<arrow::Table> getMarketViewParallel(const StockList& stks, const Datetime& date,
                                                    const string& market) {
    auto& sm = StockManager::instance();
    auto* plugin = sm.getPlugin<HkuExtraPluginInterface>(HKU_PLUGIN_HKU_EXTRA);
    HKU_ERROR_IF_RETURN(!plugin, nullptr, "Can't find {} plugin!", HKU_PLUGIN_HKU_EXTRA);
    return plugin->getMarketViewParallel(stks, date, market);
}

}  // namespace hku