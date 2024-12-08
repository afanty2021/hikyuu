/*
 *  Copyright(C) 2021 hikyuu.org
 *
 *  Create on: 2021-02-16
 *     Author: fasiondog
 */

#include <csignal>
#include <hikyuu/strategy/Strategy.h>
#include <hikyuu/strategy/BrokerTradeManager.h>
#include <hikyuu/strategy/RunSystemInStrategy.h>
#include <hikyuu/strategy/RunPortfolioInStrategy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace hku;

void export_Strategy(py::module& m) {
    py::class_<Strategy, StrategyPtr>(m, "Strategy")
      .def(py::init<>())
      .def(py::init<const vector<string>&, const vector<KQuery::KType>&, const std::string&,
                    const std::string&>(),
           py::arg("code_list"), py::arg("ktype_list"), py::arg("name") = "Strategy",
           py::arg("config") = "", R"(创建策略运行时
           
    :param list code_list: 证券代码列表，如：["sz000001", "sz000002"], "all" 代表全部证券
    :param list ktype_list: K线类型列表, 如: ["day", "min"]
    :param str name: 策略名称
    :param str config: 配置文件名称(如需要使用独立的配置文件，否则为空时使用默认的hikyuu配置文件))")

      .def(py::init<const StrategyContext&, const string&, const string&>(), py::arg("context"),
           py::arg("name") = "Strategy", py::arg("config") = "",
           R"(使用上下文创建策略运行时

    :param StrategyContext context: 上下文实例
    :param str name: 策略名称
    :param str config: 配置文件名称(如需要使用独立的配置文件，否则为空时使用默认的hikyuu配置文件))")

      .def_property("name", py::overload_cast<>(&Strategy::name, py::const_),
                    py::overload_cast<const string&>(&Strategy::name),
                    py::return_value_policy::copy, "策略名称")

      .def_property_readonly("context", &Strategy::context, py::return_value_policy::copy,
                             "获取策略上下文")

      .def("start", &Strategy::start, py::arg("auto_recieve_spot") = true, R"(start(self)

    启动策略执行，请在完成相关回调设置后执行。

    :param bool auto_recieve_spot: 是否自动接收行情数据)")

      .def(
        "on_change",
        [](Strategy& self, py::object func) {
            HKU_CHECK(py::hasattr(func, "__call__"), "func is not callable!");
            py::object c_func = func.attr("__call__");
            auto new_func = [=](const Stock& stk, const SpotRecord& spot) {
                try {
                    c_func(stk, spot);
                } catch (const std::exception& e) {
                    HKU_ERROR(e.what());
                } catch (...) {
                    HKU_ERROR("Unknown error!");
                }
            };
            self.onChange(new_func);
        },
        R"(onchang(self, func)
           
    设置证券数据更新回调通知

    :param func: 一个可调用的对象如普通函数，需接收 stock 和 ktype 参数)")

      .def(
        "on_received_spot",
        [](Strategy& self, py::object func) {
            HKU_CHECK(py::hasattr(func, "__call__"), "func is not callable!");
            py::object c_func = func.attr("__call__");
            auto new_func = [=](Datetime revTime) {
                try {
                    c_func(revTime);
                } catch (const std::exception& e) {
                    HKU_ERROR(e.what());
                } catch (...) {
                    HKU_ERROR("Unknown error!");
                }
            };
            self.onReceivedSpot(new_func);
        },
        R"(on_received_spot(self, func)

    设置证券数据更新通知回调

    :param func: 可调用对象如普通函数，没有参数)")

      .def(
        "run_daily",
        [](Strategy& self, py::object func, const TimeDelta& time, std::string market,
           bool ignore_market) {
            HKU_CHECK(py::hasattr(func, "__call__"), "func is not callable!");
            py::object c_func = func.attr("__call__");
            auto new_func = [=]() {
                try {
                    c_func();
                } catch (py::error_already_set& e) {
                    if (e.matches(PyExc_KeyboardInterrupt)) {
                        HKU_INFO("KeyboardInterrupt");
                        raise(SIGTERM);
                    }
                } catch (const std::exception& e) {
                    HKU_ERROR(e.what());
                } catch (...) {
                    HKU_ERROR("Unknown error!");
                }
            };
            self.runDaily(new_func, time, market, ignore_market);
        },
        py::arg("func"), py::arg("time"), py::arg("market") = "SH",
        py::arg("ignore_market") = false, R"(run_daily(self, func)
        
    设置日内循环执行回调。如果忽略市场开闭市，则自启动时刻开始按间隔时间循环，
    否则第一次执行时将开盘时间对齐时间间隔，且在非开市时间停止执行。

    :param func: 可调用对象如普通函数，没有参数
    :param TimeDelta time: 间隔时间，如间隔3秒：TimeDelta(0, 0, 0, 3) 或 Seconds(3)
    :param str market: 使用哪个市场的开闭市时间
    :param ignore_market: 忽略市场开闭市时间)")

      .def(
        "run_daily_at",
        [](Strategy& self, py::object func, const TimeDelta& time, bool ignore_holiday) {
            HKU_CHECK(py::hasattr(func, "__call__"), "func is not callable!");
            py::object c_func = func.attr("__call__");
            auto new_func = [=]() {
                try {
                    c_func();
                } catch (const std::exception& e) {
                    HKU_ERROR(e.what());
                } catch (...) {
                    HKU_ERROR("Unknown error!");
                }
            };
            self.runDailyAt(new_func, time, ignore_holiday);
        },
        py::arg("func"), py::arg("time"), py::arg("ignore_holiday") = true,
        R"(run_daily_at(self, func)

    设置每日定点执行回调

    :param func: 可调用对象如普通函数，没有参数
    :param TimeDelta time: 执行时刻，如每日15点：TimeDelta(0, 15)
    :param ignore_holiday: 节假日不执行)");

    m.def("crtBrokerTM", crtBrokerTM, py::arg("broker"), py::arg("cost_func") = TC_Zero(),
          py::arg("name") = "SYS", py::arg("other_brokers") = std::vector<OrderBrokerPtr>());

    m.def("run_in_strategy",
          py::overload_cast<const SYSPtr&, const Stock&, const KQuery&, const OrderBrokerPtr&,
                            const TradeCostPtr&, const std::vector<OrderBrokerPtr>&>(runInStrategy),
          py::arg("sys"), py::arg("stock"), py::arg("query"), py::arg("broker"),
          py::arg("cost_func"), py::arg("other_brokers") = std::vector<OrderBrokerPtr>(),
          R"(run_in_strategy(sys, stk, query, broker, costfunc, [other_brokers=[]])
          
    在策略运行时中执行系统交易 SYS
    目前仅支持 buy_delay| sell_delay 均为 false 的系统，即 close 时执行交易
 
    :param sys: 交易系统
    :param stk: 交易对象
    :param query: 查询条件
    :param broker: 订单代理（专用与和账户资产同步的订单代理）
    :param costfunc: 成本函数
    :param other_brokers: 其他的订单代理)");

    m.def("run_in_strategy",
          py::overload_cast<const PFPtr&, const KQuery&, int, const OrderBrokerPtr&,
                            const TradeCostPtr&, const std::vector<OrderBrokerPtr>&>(runInStrategy),
          py::arg("pf"), py::arg("query"), py::arg("adjust_cycle"), py::arg("broker"),
          py::arg("cost_func"), py::arg("other_brokers") = std::vector<OrderBrokerPtr>(),
          R"(run_in_strategy(pf, query, adjust_cycle, broker, costfunc, [other_brokers=[]])
          
    在策略运行时中执行组合策略 PF
    目前仅支持 buy_delay| sell_delay 均为 false 的系统，即 close 时执行交易

    :param Portfolio pf: 资产组合
    :param Query query: 查询条件
    :param int adjust_cycle: 调仓周期
    :param broker: 订单代理（专用与和账户资产同步的订单代理）
    :param costfunc: 成本函数
    :param other_brokers: 其他的订单代理)");

    m.def("crt_sys_strategy", crtSysStrategy, py::arg("sys"), py::arg("stk_market_code"),
          py::arg("query"), py::arg("broker"), py::arg("cost_func"),
          py::arg("other_brokers") = std::vector<OrderBrokerPtr>(), py::arg("name") = "SYSStrategy",
          py::arg("config") = "");

    m.def("crt_pf_strategy", crtPFStrategy, py::arg("pf"), py::arg("query"),
          py::arg("adjust_cycle"), py::arg("broker"), py::arg("cost_func"),
          py::arg("name") = "PFStrategy", py::arg("other_brokers") = std::vector<OrderBrokerPtr>(),
          py::arg("config") = "");

    m.def("get_data_from_buffer_server", getDataFromBufferServer);
}