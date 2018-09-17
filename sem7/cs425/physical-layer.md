# Physical Layer

## Multiplexing

There are two types of multiplexing

  1. **Frequency Division Multiplexing (FDM)**
	- Possible in the case where transmission media has a bandwidth greater
	  than the required bandwidth of signals to be transmitted
	- Two signals are allotted different frequencies with sufficient gap between
	  different frequency spectrum
	- e.g. cable TV
  2. **Time Division Multiplexing (TDM)**
	- Possible when data rate of the source is much higher than the data rate of
	  the source
	- **Synchronous TDM**: Time slots are preassigned and are fixed
	- **Synchronous TDM**: Time slots are not fixed and are allotted
	  dynamically based on speed of sources and requirement

### Network Topologies

There are majorly three kinds of topologies

  1. **Star Topology**
	- All nodes are connected to a central hub
	- Advantages
	  * Error detection is easier
	  * Network runs even if one host fails
	  * Network is scalable
	  * More suited for larger networks
	- Disadvantages
	  * Broadcasting and multicasting is not easy
	  * Network fails if central node goes down
	  * Installation costs are high
  2. **Bus Topology**:
	- All nodes are connected to a single cable (Backbone) using drop cables
	- Advantages
	  * Broadcasting and multicasting is easy
	  * Network runs even if one host fails
	  * Least expensive
	  * Good for smaller networks not requiring higher speeds
	- Disadvantages
	  * Error detection is a problem
	  * Less secure because sniffing is easier
	  * Limited in size and speed
  3. **Ring Topology**:
	- All nodes are connected to a single cable in a circle
	- Advantages
	  * Broadcasting and multicasting is easy
	  * Less expensive
	  * Guaranteed that each host will get to transmit within a finite time
		interval
	  * Performs better than a star network under heavy load
	- Disadvantages
	  * Network fails even if one host fails
	  * Error detection is a problem
	  * Difficult to move, add or change nodes
	  * Slower than star topology under normal load


