Config = {
    Application = {
        Type = "OUCH50",
        Log = {
            Level = "info",
            --File = "server.log",
        },
    },
    Session = {
        Acceptor = {
            Port = 7777,
        },
        Username = "PAXUSR",
        Password = "PAXPAS",
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
                { Quantity = 5101, Error = 1 },
                { Quantity = 5111, Error = 11 }
            },
            Replace = {
                { Quantity = 5102, Error = 2 }
            },
            Cancel = {
                { Quantity = 5103, Error = 3 }
            }
        },
        OrderCancel = {
            { Quantity = 7101, Error = 99, Message = "Unsolicited cancel at size" },
            { Quantity = 7102, Error = 98, Message = "Unsolicited cancel at size, different error" },

            { Below = 90,      Error = 90, Message = "Unsolicited cancel if below 90" },
            { Below = 80,      Error = 80, Message = "Unsolicited cancel if below 80" },
        },
        OrderFill = {
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
                    { Quantity = 303, Price = 3.45 },
                }
            },
            {
                Quantity = 6105,
                Fills = {
                    { Quantity = 1101, Price = 11.23 },
                    { Quantity = 2202, Price = 22.34 }
                }
            }
        },
        FillCancel = {
            { Quantity = 101, Price = 1.23 },
            { Quantity = 202, Price = 2.34 },
        },
    }
}
