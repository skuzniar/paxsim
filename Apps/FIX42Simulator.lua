Config = {
    Application = {
        Type = "FIX42",
        Log = {
            Level = "info",
            --File = "FIX42Server.log",
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
    Control = {
        Acceptor = {
            Port = 8888,
        },
    },
    Modules = {
        OrderReject = {
            Enter = {
                { Quantity = 5101, Error = 99, Message = "Simulating invalid order" },
                { Quantity = 5111, Error = 98, Message = "Simulating invalid order, different error" }
            },
            Replace = {
                { Quantity = 5102, Error = 99, Message = "Simulating invalid order replace" }
            },
            Cancel = {
                { Quantity = 5103, Error = 99, Message = "Simulating invalid order cancel" }
            }
        },
        OrderCancel = {
            { Quantity = 7101, Error = 99, Message = "Unsolicited cancel at size" },
            { Quantity = 7102, Error = 98, Message = "Unsolicited cancel at size, different error" },

            { Leaves = 7111, Error = 99, Message = "Unsolicited cancel at size" },
            { Leaves = 7112, Error = 98, Message = "Unsolicited cancel at size, different error" },

            { Below = 90, Error = 90, Message = "Unsolicited cancel if below 90" },
            { Below = 80, Error = 80, Message = "Unsolicited cancel if below 80" },
        },
        OrderFill = {
            {
                Quantity = 6101,
                Fills = {
                    { Quantity = 100, Price = 1.230 }
                }
            },
            {
                Quantity = 6102,
                Fills = {
                    { Quantity = 100, Price = 1.230 },
                    { Quantity = 200, Price = 2.340 }
                }
            },
            {
                Quantity = 6103,
                Fills = {
                    { Quantity = 100, Price = 1.230 },
                    { Quantity = 200, Price = 2.340 },
                    { Quantity = 300, Price = 3.450 }
                }
            },
            {
                Quantity = 6104,
                Fills = {
                    { Quantity = 101, Price = 1.230 },
                    { Quantity = 202, Price = 2.340 },
                }
            },
            {
                Quantity = 6105,
                Fills = {
                    { Quantity = 1101, Price = 11.230 },
                    { Quantity = 2202, Price = 22.340 }
                }
            }
        },
        FillCancel = {
            { Quantity = 101, Price = 1.230 },
            { Quantity = 202, Price = 2.340 },
        },
        FillCorrect = {
            { Quantity = 1101, Price = 11.230, NewQuantity = 1011, NewPrice = 1.240 },
            { Quantity = 2202, Price = 22.340, NewQuantity = 2022, NewPrice = 2.350 }
        }
    }
}
