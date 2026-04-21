Config = {
    Application = {
        Type = "FIX42",
        Log = {
            Level = "info",
            --File = "FIX42Client.log"
        },
    },
    Session = {
        Connector = {
            Host = "localhost",
            Port = 7777,
            Retries = 10,
            Delay = 1,
        },
        BeginString = 'FIX.4.2',
        TargetCompID = 'PAXSIM-SERVER',
        SenderCompID = 'PAXSIM-CLIENT',
        HBInterval = 10,
    },
}
