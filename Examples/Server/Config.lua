Config = {
    Server = {
        Type = "Fix42",
        Log = {
            Level = "info",
            --File = "server.log",
        },
    },
    Session = {
        Acceptor = {
            Port = 7777,
        },
        BeginString = 'FIX.4.2',
        TargetCompID = 'PAXSIM-CLIENT',
        SenderCompID = 'PAXSIM-SERVER',
        HBInterval = 10,
    },
    OrderFlow = {
        Reject = {
            Enter = {
                Quantity = 5101, ErrorCode = 99, Message = "Simulating invalid order"
            },
            Replace = {
                Quantity = 5102, ErrorCode = 99, Message = "Simulating invalid order replace"
            },
            Cancel = {
                Quantity = 5103, ErrorCode = 99, Message = "Simulating invalid order cancel"
            }
        }
    },
    OrderBook = {
        Executions = {
            {
                Quantity = 6101,
                Fills = {
                    { Quantity = 100, Price = 1.23 }
                }
            },
            {
                Quantity = 6102,
                Fills = {
                    { Quantity = 100, Price = 1.23 },
                    { Quantity = 200, Price = 2.34 }
                }
            },
            {
                Quantity = 6103,
                Fills = {
                    { Quantity = 100, Price = 1.23 },
                    { Quantity = 200, Price = 2.34 },
                    { Quantity = 300, Price = 3.45 }
                }
            },
            {
                Quantity = 6104,
                Fills = {
                    { Quantity = 101, Price = 1.23 },
                    { Quantity = 202, Price = 2.34 },
                }
            },
            {
                Quantity = 6105,
                Fills = {
                    { Quantity = 1101, Price = 11.23 },
                    { Quantity = 2202, Price = 22.34 }
                }
            }
        }
    },
    ExecutionBook = {
        Cancellations = {
            { Quantity = 101, Price = 1.23 },
            { Quantity = 202, Price = 2.34 },
        },
        Corrections = {
            { Quantity = 1101, Price = 11.23, NewQuantity = 1011, NewPrice = 1.23 },
            { Quantity = 2202, Price = 22.34, NewQuantity = 2022, NewPrice = 2.34 }
        }
    }
}
