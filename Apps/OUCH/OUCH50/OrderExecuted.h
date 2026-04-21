#ifndef OrderExecuted_h
#define OrderExecuted_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderExecuted
{
    static constexpr char Type = 'E';

    SequencedData       header = { Type, sizeof(OrderExecuted) };
    Timestamp           timestamp;
    UserRefNum          userRefNum;
    Int32               executedQuantity;
    Price               executionPrice;
    LiquidityFlag       liquidityFlag;
    Int32               matchNumber;
    Alpha<4>            contraFirm;
    TradingMode         tradingMode;
    TransactionCategory transactionCategory;
    AlgoIndicator       algoIndicator;
    LiquidityAttributes liquidityAttributes;
    Market              lastMarket;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const OrderExecuted& o)
{
    // clang-format off
    s << "[OrderExecuted]=";
    s << "Header: "              << o.header              << ' ';
    s << "Timestamp: "           << o.timestamp           << ' ';
    s << "UserRefNum: "          << o.userRefNum          << ' ';
    s << "ExecutedQuantity: "    << o.executedQuantity    << ' ';
    s << "ExecutionPrice: "      << o.executionPrice      << ' ';
    s << "LiquidityFlag: "       << o.liquidityFlag       << ' ';
    s << "MatchNumber: "         << o.matchNumber         << ' ';
    s << "ContraFirm: "          << o.contraFirm          << ' ';
    s << "TradingMode: "         << o.tradingMode         << ' ';
    s << "TransactionCategory: " << o.transactionCategory << ' ';
    s << "AlgoIndicator: "       << o.algoIndicator       << ' ';
    s << "LiquidityAttributes: " << o.liquidityAttributes << ' ';
    s << "LastMarket: "          << o.lastMarket;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
