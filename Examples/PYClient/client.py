import sys
import argparse
import quickfix

from fixapp import Application

def main(config, path):
    try:
        settings    = quickfix.SessionSettings(config)
        application = Application()

        storefactory = quickfix.FileStoreFactory(settings)
        logfactory   = quickfix.FileLogFactory(settings)
        initiator    = quickfix.SocketInitiator(application, storefactory, settings, logfactory)

        initiator.start()
        application.run(path)
        initiator.stop()

    except (quickfix.ConfigError, quickfix.RuntimeError) as e:
        print(e)
        initiator.stop()
        sys.exit()

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='FIX Client')
    parser.add_argument('config', type=str, help='Name of configuration file')
    parser.add_argument('path',   type=str, help='Test cases path')
    args = parser.parse_args()
    main(args.config, args.path)
