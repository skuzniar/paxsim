Sample programs to demonstrate the use of the core library components.

* Echo-1
Echo server that reads input from the client and echoes back the same content. 
Message handler consist of a single class that acts as the source and the target.

* Echo-2
Echo server that reads input from the client and echoes back the same content. 
Message handler consist of two classes. One acts as the source of messages, the
other as the target.

* Echo-3
Echo server that reads input from the client and echoes back the formatted content. 
Message handler consist of three classes. The first acts as the source of messages. 
The second formats the message. The third is the target.

* Echo-4
Same as Echo-3 but introduces Flareout class as the middle layer of the pipeline.

* Echo-5
Echo server that reads input from the client and echoes back the formatted content. 
Message handler consist of four classes. The first acts as the source of messages. 
The second and third format the message. The fourth class is the target. The program
also demonstrates the use of timer events. Periodically the source module will prompt
the client to complete the sentence 

* Echo-6
Same as Echo-5 but introduces the Context class. The Context class is shared among
the session modules and it stays alive after the session terminates. This is used to 
preserve state across multiple sessions. The smaple program demonstrates this by
counting the total number of sentences sent by the client.

* Echo-7
Same as Echo-6 but introduces the Config class. The Config class is constructed
up-front and passed to every context component. Message processing modules get it 
from context and use it for their own configuration.

