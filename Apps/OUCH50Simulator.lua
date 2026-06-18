Config = {
    Application = {
        Type = "OUCH50",
        Log = {
            Level = "debug",
            --File = "server.log",
        },
    },
    Session = {
        Acceptor = {
            Port = 7777,
        },
        UserName = "PAXUSR",
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
            { Quantity = 7101, Reason = "Supervisory" },
            { Quantity = 7102, Reason = "UserRequested" },

            { Below = 90,      Reason = "State" },
            { Below = 80,      Reason = "BadQuote" },
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
            { Quantity = 101, Price = 1.23, Reason = "Consent" },
            { Quantity = 202, Price = 2.34, Reason = "Erroneous" },
        },
    }
}
