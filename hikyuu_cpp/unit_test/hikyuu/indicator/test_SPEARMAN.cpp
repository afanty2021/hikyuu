/*
 * test_COS.cpp
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-5-1
 *      Author: fasiondog
 */

#include "../test_config.h"
#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_COS test_indicator_CORR
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

static void spearmanLevel(const IndicatorImp::value_t *data, IndicatorImp::value_t *level,
                          size_t total) {
    std::vector<std::pair<IndicatorImp::value_t, size_t>> data_index(total);
    for (size_t i = 0; i < total; i++) {
        data_index[i].first = data[i];
        data_index[i].second = i;
    }

    std::sort(
      data_index.begin(), data_index.end(),
      std::bind(
        std::less<IndicatorImp::value_t>(),
        std::bind(&std::pair<IndicatorImp::value_t, size_t>::first, std::placeholders::_1),
        std::bind(&std::pair<IndicatorImp::value_t, size_t>::first, std::placeholders::_2)));

    size_t i = 0;
    while (i < total) {
        size_t count = 1;
        IndicatorImp::value_t score = i + 1.0;
        for (size_t j = i + 1; j < total; j++) {
            if (data_index[i].first != data_index[j].first) {
                break;
            }
            count++;
            score += j + 1;
        }
        score = score / count;
        for (size_t j = 0; j < count; j++) {
            level[data_index[i + j].second] = score;
        }
        i += count;
    }
}

/** @par 检测点 */
TEST_CASE("test_spearmanLevel") {
    std::vector<IndicatorImp::value_t> a{3., 8., 4., 7., 2.};
    size_t totala = a.size();
    auto levela = std::make_unique<IndicatorImp::value_t[]>(totala);
    auto *ptra = levela.get();
    spearmanLevel(a.data(), levela.get(), totala);

    std::vector<IndicatorImp::value_t> expecta = {2., 5., 3., 4., 1.};
    for (size_t i = 0; i < totala; i++) {
        CHECK_EQ(ptra[i], doctest::Approx(expecta[i]));
    }

    std::vector<IndicatorImp::value_t> b{5., 10., 8., 10., 6.};
    size_t totalb = b.size();
    auto levelb = std::make_unique<IndicatorImp::value_t[]>(totalb);
    auto *ptrb = levelb.get();
    spearmanLevel(b.data(), levelb.get(), totalb);

    std::vector<IndicatorImp::value_t> expectb = {1., 4.5, 3., 4.5, 2.};
    for (size_t i = 0; i < totalb; i++) {
        CHECK_EQ(ptrb[i], doctest::Approx(expectb[i]));
    }
}

/** @par 检测点 */
TEST_CASE("test_SPEARMAN") {
    Indicator result;

    // 空指标
    result = SPEARMAN(Indicator(), Indicator(), 10);
    CHECK_UNARY(result.empty());

    PriceList a{3., 8., 4., 7., 2.};
    PriceList b{5., 10., 8., 10., 6.};

    Indicator x = PRICELIST(a);
    Indicator y = PRICELIST(b);

    // 非法参数 n
    result = SPEARMAN(x, y, 0);
    CHECK_UNARY(result.empty());
    result = SPEARMAN(x, y, 1);
    CHECK_UNARY(result.empty());

    // 正常情况
    PriceList expect{Null<price_t>(), 1., 1., 0.95, 0.875};
    result = SPEARMAN(x, y, a.size());
    CHECK_EQ(result.name(), "SPEARMAN");
    CHECK_EQ(result.discard(), 4);
    CHECK_EQ(result.size(), a.size());
    for (size_t i = result.discard(); i < result.size(); i++) {
        CHECK_EQ(result[i], doctest::Approx(expect[i]));
    }

    expect = {Null<price_t>(), Null<price_t>(), 1., 0.875, 1.};
    result = SPEARMAN(x, y, 3);
    CHECK_EQ(result.name(), "SPEARMAN");
    CHECK_EQ(result.discard(), 2);
    CHECK_EQ(result.size(), a.size());
    for (size_t i = result.discard(); i < result.size(); i++) {
        CHECK_EQ(result[i], doctest::Approx(expect[i]));
    }
}

//-----------------------------------------------------------------------------
// benchmark
//-----------------------------------------------------------------------------
#if ENABLE_BENCHMARK_TEST
TEST_CASE("test_SPEARMAN_benchmark") {
    Stock stock = getStock("sh000001");
    KData kdata = stock.getKData(KQuery(0));
    Indicator c = kdata.close();
    Indicator h = kdata.close();
    int cycle = 10;  // 测试循环次数

    {
        BENCHMARK_TIME_MSG(test_SPEARMAN_benchmark, cycle, fmt::format("data len: {}", c.size()));
        SPEND_TIME_CONTROL(false);
        for (int i = 0; i < cycle; i++) {
            Indicator result = SPEARMAN(c, h, 200);
        }
    }
}
#endif

//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
TEST_CASE("test_SPEARMAN_export") {
    StockManager &sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/SPEARMAN.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = SPEARMAN(CLOSE(kdata), OPEN(kdata), 10);
    {
        std::ofstream ofs(filename);
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(x1);
    }

    Indicator x2;
    {
        std::ifstream ifs(filename);
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(x2);
    }

    CHECK_EQ(x2.name(), "SPEARMAN");
    CHECK_EQ(x1.size(), x2.size());
    CHECK_EQ(x1.discard(), x2.discard());
    CHECK_EQ(x1.getResultNumber(), x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        if (std::isnan(x1[i])) {
            CHECK_UNARY(std::isnan(x2[i]));
        } else {
            CHECK_EQ(x1[i], doctest::Approx(x2[i]));
        }
    }

    Indicator x11 = x1.getResult(1);
    Indicator x21 = x2.getResult(1);
    CHECK_EQ(x11.size(), x21.size());
    CHECK_EQ(x11.discard(), x21.discard());
    CHECK_EQ(x11.getResultNumber(), x21.getResultNumber());
    for (size_t i = x11.discard(); i < x21.size(); ++i) {
        if (std::isnan(x11[i])) {
            CHECK_UNARY(std::isnan(x21[i]));
        } else {
            CHECK_EQ(x11[i], doctest::Approx(x21[i]));
        }
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */
