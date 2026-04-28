Config = {
    Application = {
        Type = "OUCH50",
        Log = {
            Level = "info",
            --File = "OUCH50Client.log"
        },
    },
    Session = {
        Connector = {
            Host = "localhost",
            Port = 7777,
            Retries = 10,
            Delay = 1,
        },
        UserName = "PAXUSR",
        Password = "PAXPAS",
        HBInterval = 10,
    },
}
