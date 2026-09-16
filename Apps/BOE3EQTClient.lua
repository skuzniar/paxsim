Config = {
    Application = {
        Type = "BOE3EQT",
        Log = {
            Level = "info",
            --File = "BOE3EQTClient.log"
        },
    },
    Session = {
        Connector = {
            Host = "localhost",
            Port = 7777,
            Retries = 10,
            Delay = 1,
        },
        UserName = "PAXU",
        Password = "PAXPASSWD",
        HBInterval = 10,
    },
}
