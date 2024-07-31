# Control module.

Control module is a collection control commands that drive the behavior of various part of the simulator. The control
flow of the simulator requires that there is a single command type. 

# Order Control module.

Order Control module defines commands that operate on orders. The commands are produced by the controller and consumed
by the modules responsible for maintaining the order book.

# Execution Control module.

Execution Control module defines commands that operate on executions. The commands are produced by the controller and 
consumed by the modules responsible for maintaining the execution book.
