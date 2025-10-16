/*
 *  Copyright (c) 2025 hikyuu.org
 *
 *  Created on: 2025-10-17
 *      Author: fasiondog
 */

#pragma once

#include "misc.h"

namespace hku {

vector<FundsList> HKU_API parallel_run_sys(const SystemList& system_list, const KQuery& query,
                                           bool reset, bool resetAll) {
    return parallel_for_index(0, system_list.size(), [&](size_t i) {
        FundsList funds;
        if (system_list[i]) {
            system_list[i]->run(query, reset, resetAll);
            auto tm = system_list[i]->getTM();
            if (tm) {
                auto dates = system_list[i]->getTO().getDatetimeList();
                if (!dates.empty()) {
                    funds = tm->getFundsList(dates, query.kType());
                }
            }
        }
        return funds;
    });
}

vector<FundsList> HKU_API parallel_run_pf(const vector<PFPtr>& pf_list, const KQuery& query,
                                          bool force) {
    return parallel_for_index(0, pf_list.size(), [&](size_t i) {
        FundsList funds;
        if (pf_list[i]) {
            pf_list[i]->run(query, force);
            auto tm = pf_list[i]->getTM();
            if (tm) {
                auto dates = StockManager::instance().getTradingCalendar(query, "SH");
                if (!dates.empty()) {
                    funds = tm->getFundsList(dates, query.kType());
                }
            }
        }
        return funds;
    });
}

}  // namespace hku