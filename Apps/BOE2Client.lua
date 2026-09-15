Config = {
    Application = {
        Type = "BOE2",
        Log = {
            Level = "info",
            --File = "BOE2Client.log"
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
