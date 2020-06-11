Sonar is a tool developed by Varun, which can be found here:

    https://github.com/sharm294/sonar

Sonar can do two different things:

    1) Automatically generate a SystemVerilog testbench from a simple set
       of Python commands (currently includes support for raw signals, AXI
       Stream interfaces, and AXI Lite interfaces)
    
    2) Manage all your HLS/RTL source files and automatically generate all
       the necessary Vivado scripts to simulate your design. (This will also
       include future support for package/dependency management)
    
To be more precise, sonar is a _library_ that you call from inside Python to
describe and ultimately output a SystemVerilog testbench. 

Although part 2 is a desperately needed feature, this tutorial only covers
part 1.

All files used in the running example have been added alongside this README
for your convenience.

========
OVERVIEW
========

Using sonar involves the following steps:

    1) Create a new Python script, and import the Sonar libraries
    2) Define the input/output ports and interfaces on the design you wish
       to simulate
    3) Define the test inputs and expected outputs
    4) Run the Python script; this will generate the SystemVerilog testbench
       and a data file that contains your test data
    5) Run the simulation in Vivado

Each of the above steps has its own section in this document. First, we'll
start with a running example.

===============
RUNNING EXAMPLE
===============

Create the following Verilog file in a fresh directory. This module takes in
an AXI Stream and, if the enable_swap input is high, swaps the endianness
before outputting.

    axis_swap_endian.v
    ~~~~~~~~~~~~~~~~~~
    
    module axis_swap_endian (
        input wire clk,
        input wire rst,
        
        input wire enable_swap,
        
        //For sonar (and Vivado) to properly use AXI Stream, must make sure
        //all wires in an interface have the form "name_TCHANNEL"
        input wire [31:0] old_TDATA,
        input wire [3:0] old_TKEEP,
        input wire old_TLAST,
        input wire old_TVALID,
        output wire old_TREADY,
        
        output wire [31:0] new_TDATA,
        output wire [3:0] new_TKEEP,
        output wire new_TLAST,
        output wire new_TVALID,
        input wire new_TREADY
    );
    
        assign new_TDATA = enable_swap ?
            {old_TDATA[7:0], old_TDATA[15:8], old_TDATA[23:16], old_TDATA[31:24]}
            : old_TDATA
        ;
        
        assign new_TKEEP = enable_swap ?
            {old_TKEEP[0], old_TKEEP[1], old_TKEEP[2], old_TKEEP[3]}
            : old_TKEEP
        ;
        
        assign new_TLAST = old_TLAST;
    
        assign new_TVALID = old_TVALID;
        
        assign old_TREADY = new_TREADY;
    
    endmodule

We will slowly build up the Python testbench file. For convenience, the
entire file is provided at the end of this document.

=======================================
CREATE PYTHON FILE AND IMPORT LIBRARIES
=======================================

To use sonar, you write a Python script that calls certain library 
functions. Over the course of the next few sections we'll gradually build 
up the final Python script. From now on, append all code samples to the end
of your script.

    gen_tb.py
    ~~~~~~~~~
    # Import sonar libraries
    from sonar.testbench import Testbench, Module, TestVector, Thread
    from sonar.interfaces import AXIS, SAXILite
    
    # Also import os so that we can get filepaths
    import os

Let's briefly explain the different sonar components.

    Testbench:  The "top-level" object that contains all metadata needed to
                generate the SystemVerilog. All other objects will 
                eventually become children of this one. Specifically, a 
                Testbench contains one Module and a list of TestVectors.
                
    Module:     Represents the module you are testing. This object contains
                information about the clocks, resets, and raw ports (i.e. 
                wires not belonging to any bus interface) of your module. 
    
    TestVector: This is essentially one "unit test" in your simulation. The
                Testbench object contains a list of TestVectors, which are
                each executed one after the other. A TestVector contains a 
                list of Thread objects (described below).
                
    Thread:     This represents one "serial" thread of events in a 
                TestVector. In a single TestVector, all Threads are being 
                simulated in parallel, but all events within a Thread are 
                simulated sequentially.

Finally, we have the interface objects (AXIS and SAXILite). As it happens, 
you technically don't need these objects; the low-level operations available
to you for Module and Thread objects are capable of simulating pretty much
anything you want. However, these objects _automate_ the otherwise tedious
lines of code you would need to write to manage individual bus wire and 
transactions.


=============================================
DESCRIBE YOUR MODULE AND ITS PORTS/INTERFACES
=============================================

The next step is to define the design you are testing. You do this by 
creating and filling a Module object. The code for our running example is
given below, and explanations of each function follow below:
    
    gen_tb.py (continued)
    ~~~~~~~~~~~~~~~~~~~~~
    
    # Instantiate a Module object
    dut = Module.default("DUT")
    
    # Define clocks, resets, and raw wires
    dut.add_clock_port("clk", "10ns")
    dut.add_reset_port("rst")
    dut.add_port("enable_swap", size=1, direction="input")
    
    # Create AXIS objects for the input and output
    # We'll do this AXIS interface the "hard way"...
    old = AXIS("old", "slave", "clk")
    old.port.init_channels("empty")
    old.port.add_channel("TDATA", "tdata", 32)
    old.port.add_channel("TVALID", "tvalid")
    old.port.add_channel("TREADY", "tready")
    old.port.add_channel("TLAST", "tlast")
    old.port.add_channel("TKEEP", "tkeep", 4)
    #...and this AXIS interface the "easy way"
    new = AXIS("new", "master", "clk")
    # The "tkeep" mode of init channels automatically adds all the signals
    # that we chose to do manually for the first AXIS interface
    new.port.init_channels("tkeep", 32)
    
    # Add the AXIS objects to the Module
    dut.add_interface(old)
    dut.add_interface(new)

The first step is to instantiate a Module object. This is done using the 
Module.default function. The argument is the desired instance name, which 
for now MUST be "DUT". In other words, there is nothing you can change about
this function call.

Next, we define clocks, resets, and raw wires. 

 -> The add_clock_port function takes two arguments: the first is the 
    actual name of the port in the Verilog, and the second is a string 
    giving the clock period to use in the simulation.
    
 -> The add_reset_port function only takes the name of the port
    
 -> The add_port function takes the name, size, and direction of the port.

We move on to creating objects that describe the bus interfaces on our 
module. 

 -> The AXIS constructor takes three arguments: the name of the interface 
    (i.e. the part before the underscore in name_TCHANNEL), the direction 
    of the interface ("master" or "slave") and the name of the associated 
    clock port.
    
 -> We must next describe which channels are present and what their size is. 
    We must first call init_channels. This initializes some internal data
    structures, and takes two arguments. The first is used to say which 
    channels are present in this interface. It is one of:
    
        - "default", which include tdata, tvalid, tready, and tlast
        
        - "tkeep", which includes everything from "default" and tkeep
        
        - "min", which includes only tdata and tvalid
        
        - "empty", which contains nothing. In this case, the second argument
          is not given.

    The second argument is the width of the TDATA channel. The TKEEP width
    is automatically inferred.
    
 -> However, suppose that you need a specific set of channels which isn't 
    covered by the above choices for init_channels. No problem: you can add 
    your own channels with add_channel. The first argument is what comes 
    after the underscorde in name_TCHANNEL in your Verilog code. The second 
    argument is either "tdata", "tvalid", "tready", "tlast", "tkeep", or 
    "tdest"; this is used to connect the wire in your Verilog module to the
    connect driver in the simulation. Finally, the third argument is the 
    width, which defaults to 1 bit.

Finally, we use dut.add_interface(X) to add these AXIS interfaces to the 
definition of our module.


=======================================
DEFINE TEST INPUTS AND EXPECTED OUTPUTS
=======================================

The general idea of sonar is to have a number of TestVectors. Each 
TestVector contains one or more parallel Threads, and each Thread is 
itself a list of (sequential) operations.

We now add the following lines into our testbench:

    gen_tb.py (continued)
    ~~~~~~~~~~~~~~~~~~~~~
    
    # Create a new TestVector to represent this unit test. Eventually, we
    # will add threads into this TestVector, and later, we will add it into
    # a TestBench object
    tv = TestVector()
    
    # Create two Threads that will run in parallel
    inputs_thd = Thread()
    outputs_thd = Thread()
    

Now we will move on to using the member functions of a Thread object to add
some events into our simulation.

THREAD OPERATIONS
-----------------
The basic operations available to you in a Thread are given below. All I did
was to copy-paste Varun's docstrings into this file, along with any extra 
info I thought would be helpful:

add_delay(delay): 
    Add a timed delay to the thread

    Args:
        delay (str): String representing delay length e.g. '40ns'


set_signal(name, value):
    Set the value of a signal to be the specified value

    Args:
        name (str): Name of the signal
        value (number): May be an int, or a hex or binary number string
            (preceded by 0x or 0b respectively)


init_signals():
    Initialize all signals to zero
    
    Note: 
        Behind the scenes, sonar scans all the ports and interfaces on your 
        Module for input ports. It is these ports that will be set to zero.


enable_timestamps(prefix, index):
    Each subsequent command (until disabled) will print the time after it finishes
    with the given prefix and an index which starts at the provided one.

    Args:
        prefix (str): String to prefix the timestamp with
        index (int): Integer to start indexing the timestamps at
    
    Note:
        This is where "sonar" gets its name. Essentially, you send out a
        stimulus and then measure how much time it took for it to come back
    

disable_timestamps():
    Disables timestamping


init_timer():
    Set the timer to zero to begin timestamping


print_elapsed_time(id):
    Prints the elapsed time since the last init_timer command

    Args:
        id (str): String to print out with the timestamp for identification
        

print_time():
    Prints the absolute time


display(string):
    Print the string to the console (note, must not contain spaces)

    Args:
        string (str): String to print


end_vector():
    Ends the TestVector. This command must be the last chronological event
    in the vector. If using C++ (or any other sequential simulation), the
    thread containing this command must also be the last thread in the
    vector
    

set_flag(id):
    Set the flag with the given ID to 1. The number of flags available is
    set in the metadata of the testbench. Flags can be used to synchronize
    between threads. e.g. one thread can set a flag that another will wait
    for.

    Args:
        id (number): ID of the flag to set (ranges from 0 to Flags-1)

wait_flag(id):
    Wait for the flag with the given ID to become 1. The number of flags
    available is set in the metadata of the testbench. Flags can be used to
    synchronize between threads. e.g. one thread can set a flag that another
    will wait for.

    Args:
        id (number): ID of the flag to wait on (ranges from 0 to Flags-1)
        

wait(condition, value=None):
    Add a wait condition to the thread. For now, the condition must be a
    complete SystemVerilog line that will be inserted verbatim into the TB.
    The terminating semicolon should be included.

    Args:
        condition (str): SV-compatible wait statement (e.g. wait(); or @();)
        value (number, optional): Defaults to None. The condition can use
            '$value' as a variable and pass the number that should be
            inserted. e.g. wait(signal == $value)


wait_level(condition, value=None):
    Add a level-wait condition (wait until the signal value matches) to the
    thread.

    Args:
        condition (str): SV-compatible statement (e.g. 'signal == 1')
        value (number, optional): Defaults to None. The condition can use
            '$value' as a variable and pass the number that should be
            inserted. e.g. 'signal == $value'
            

wait_posedge(self, signal):
    Add a positive-edge sensitive wait condition on a signal.

    Args:
        signal (str): Name of the signal to wait on


wait_negedge(self, signal):
    Add a negative-edge sensitive wait condition on a signal.

    Args:
        signal (str): Name of the signal to wait on
    






ADDING INTERFACE TRANSACTIONS TO A THREAD
-----------------------------------------

Recall that an interface (such as AXIS or SAXILite) is a special object 
that automates tedious use of the lower-level Module and Thread functions.
For that reason, all of these functions take a Thread object as their first
argument; in other words, these functions will automatically invoke the
low-level Thread operations so you don't have to.


This tutorial doesn't cover SAXILite, but here is some information about
the member functions of AXIS objects:



write(thread, data, **kwargs):
    Writes the given command to the AXI stream.

    Args:
        data (str): Data to write
        kwargs (str): keyworded arguments where the keyword is the AXIS
            channel or special keyword and is assigned to the given value
    Returns:
        dict: Dictionary representing the data transaction
    
    Note: 
        An example of using the kwargs would be something like:
            write(my_thd, 0xDEADBEEF, tdest=8, tlast=1)
        
        
writes(thread, data):
    Writes an array of commands to the AXI stream. This command results in
    a smaller final file size than using the write command in a loop

    Args:
        thread (Thread): The thread to write the commands to
        data (Iterable): This should be an iterable of kwargs where the
            keywords are AXIS compliant.
    
    Note:
        These AXIS-compliant dictionaries might look like:
            {
                "tdata": 0xDEADBEEF, 
                "tdest": 8, 
                "tlast": 1
            }
            

read(thread, data, **kwargs):
    Reads the given keyworded args from an AXIS stream to verify output.

    Returns:
        dict: Dictionary representing the data transaction
    
    Note:
        Just to be clear: this function causes a flit to be read from the
        stream, and the TDATA and other sidechannels will be compared 
        against the arguments to this functions. For example, if I used
            read(my_thd, 0xDEADBEEF, tdest=8, tlast=1)
        it means I'm expecting to see 0xDEADBEEF with TDEST=8 and TLAST=1, 
        and if I don't get it, I want to see an error message.
        

reads(thread, data):
    Reads the list of keyworded args from an AXI stream to verify output.
    This command results in a smaller file size than repeated 'read' commands.

    Args:
        thread (Thread): Thread to read the data in
        data (iterable): This should be an iterable of kwargs where the
            keywords are AXIS compliant.

    Note:
        See extra notes on write, writes, and reads


wait(thread, data, bit_range=None):
    Adds a wait statement to the provided thread for a specific tdata value

    Args:
        thread (Thread): The thread to add the wait to
        data (number): The value of tdata to wait for
        bit_range (string, optional): Defaults to all bits. Range of bits
            to check in tdata, separated by a colon. e.g. "63:40"


file_to_stream(thread, filePath, parsingFunc=None, endian="little"):
    Converts the provided file into a series of AXIS transactions.

    Args:
        thread (Thread): Thread to stream the file in
        filePath (str): Path to the file to stream
        parsingFunc (Func, optional): Defaults to None. Function that
            determines how the file is parsed. Must return a list of dicts
            representing valid AXIS transactions. The default function
            assumes a binary file containing only tdata
        endian (str, optional): Defaults to 'little'. Must be little|big

    Raises:
        NotImplementedError: Unhandled exception

    Returns:
        dict: Dictionary representing the data transaction







ACTUALLY CONTINUING OUR EXAMPLE
-------------------------------

So here we'll send a few flits with endianness-swapping disabled, then we'll
send a few with endianness-swapping enabled.

    gen_tb.py (continued)
    ~~~~~~~~~~~~~~~~~~~~~
    
    # INPUT THREAD
    # Give everything an initial value
    inputs_thd.init_signals()
    # Trigger a reset
    inputs_thd.set_signal("rst", "1")
    inputs_thd.wait_negedge("clk")
    inputs_thd.set_signal("rst", "0")
    # For simplicity, pretend output AXI Stream is always ready
    # Note: for some reason we have to use lower-case "_tready". This
    # is only true for ports inside interfaces; in general, the case must
    # match whatever you have in your Verilog
    inputs_thd.set_signal("new_tready", "1")
    # Write a few flits with no byte swapping
    inputs_thd.set_signal("enable_swap", "0")
    old.write(inputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
    old.write(inputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
    # Write a few flits with byte swapping
    inputs_thd.wait_negedge("clk")
    inputs_thd.set_signal("enable_swap", "1")
    old.write(inputs_thd, "0x01234567", tkeep="0xF", tlast="0")
    old.write(inputs_thd, "0x89ABCDEF", tkeep="0xC", tlast="1")
    # Finally, signal that we're finished by setting flag 0
    inputs_thd.set_flag(0)

    # OUTPUT THREAD
    # Read flits and state our expectations
    new.read(outputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
    new.read(outputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
    new.read(outputs_thd, "0x67452301", tkeep="0xF", tlast="0")
    new.read(outputs_thd, "0xEFCDAB89", tkeep="0x3", tlast="1")
    # Wait for flag 0 to make sure the input thread is finsihed
    outputs_thd.wait_flag(0)
    # Finally, end the simulation
    outputs_thd.end_vector()
    
    # Add these two threads to the TestVector
    tv.add_thread(inputs_thd)
    tv.add_thread(outputs_thd)

Most things in here are self-explanatory, but one thing deserves mention: 
the use of flag 0 and end_vector. Basically, if you never call end_vector,
the simulation will run forever. end_vector inserts a special event into the
simulation that ends the test, but here's the problem: the test ends as soon
as it sees the first instance of end_vector. So, we use flags in kind of the
same way that you might use pthread_join. Hopefully that makes sense.

By the way: you might have issues with simulations running forever. One way
to prevent this is to have a separate Thread which simply ends the 
simulation after a maxium amount of time:

    timeout_thd = Thread()
    timeout_thd.add_delay("5000ns")
    timeout_thd.display("Simulation timed out!")
    timeout_thd.end_vector()
    
    tv.add_thread(timeout_thd)

    
===========================================================
GENERATE THE SYSTEMVERILOG TESTBENCH AND RUN THE SIMULATION
===========================================================

We're almost at the finish line. We just need to add a few more lines to our
Python testbench:

    gen_tb.py (continued)
    ~~~~~~~~~~~~~~~~~~~~~
    
    # Construct the Testbench object
    tb = Testbench.default("axis_swap_endian")
    tb.add_module(dut)
    tb.add_test_vector(tv)
    
    # Generate the SystemVerilog testbench
    tb.generateTB(os.getcwd() + "/", "sv")

The argument to Testbench.default must be the module name of the thing you
are trying to simulate. add_module and add_test_vector are self-explanatory.

generateTB takes two arguments: the first is the path to where you want the 
generated testbench files to go. I use os.getcwd() so that they end up in 
my current directory. The second argument is which language to use; I'm 
pretty sure you can only use "sv" here.

Now: run this Python script

    $ python gen_tb.py
    
With any luck, that should just work. You should now see three json files
(ignore these) and two simulation files: a .sv and a .dat. Basically, the
.sv file is what runs the simulation, and the .dat contains the test inputs.

At this point, you have all the files you need to run a simulation. Vivado,
ModelSim, and any other SystemVerilog-compliant simulator should be able to
perform your simulation, but for completeness, Vivado-specific instructions
are given below:

    $ xvlog axis_swap_endian.v
    $ xvlog --sv --relax axis_swap_endian_tb.sv
    $ xelab -debug typical axis_swap_endian_tb
    $ xsim -R axis_swap_endian_tb

Alternatively, you can simply add the .sv and .dat files to the sources of
a Vivado project and just use the GUI in the normal way.

That's it!

==================================================
LIST OF THINGS IN SONAR I HAVE NOT YET TRIED USING
==================================================

    - AXI Lite
    - The "reads" and "writes" functions
    - The automatic project management

========================
LIST OF FEATURES MISSING
========================
    
    - (added in dev branch) Setting parameters on module

    - (added in dev branch) Generated TB for AXIS reads doesn't check for 
      correct values on sidechannels
    
    - Simulation should have a maximum running time (if you have a bug and
      an AXIS flit is never generated but hte TB expects one, it just goes
      into an infinite loop)
    
    - Consider saving data with a ".mem" extension; this works much nicer in
      Vivado projects

==============================
FINAL PYTHON TESTBENCH EXAMPLE
==============================

# Import sonar libraries
from sonar.testbench import Testbench, Module, TestVector, Thread
from sonar.interfaces import AXIS, SAXILite

# Also import os so that we can get filepaths
import os

# Describe the device under test -------------------------------------------

# Instantiate a Module object
dut = Module.default("DUT")

# Define clocks, resets, and raw wires
dut.add_clock_port("clk", "10ns")
dut.add_reset_port("rst")
dut.add_port("enable_swap", size=1, direction="input")

# Create AXIS objects for the input and output
# We'll do this AXIS interface the "hard way"...
old = AXIS("old", "slave", "clk")
old.port.init_channels("empty")
old.port.add_channel("TDATA", "tdata", 32)
old.port.add_channel("TVALID", "tvalid")
old.port.add_channel("TREADY", "tready")
old.port.add_channel("TLAST", "tlast")
old.port.add_channel("TKEEP", "tkeep", 4)
#...and this AXIS interface the "easy way"
new = AXIS("new", "master", "clk")
# The "tkeep" mode of init channels automatically adds all the signals
# that we chose to do manually for the first AXIS interface
new.port.init_channels("tkeep", 32)

# Add the AXIS objects to the Module
dut.add_interface(old)
dut.add_interface(new)


# Define test inputs and expected outputs ----------------------------------

# Create a new TestVector to represent this unit test. Eventually, we
# will add threads into this TestVector, and later, we will add it into
# a TestBench object
tv = TestVector()

# Create two Threads that will run in parallel
inputs_thd = Thread()
outputs_thd = Thread()

# INPUT THREAD
# Give everything an initial value
inputs_thd.init_signals()
# Trigger a reset
inputs_thd.set_signal("rst", "1")
inputs_thd.wait_negedge("clk")
inputs_thd.set_signal("rst", "0")
# For simplicity, pretend output AXI Stream is always ready
# Note: for some reason we have to use lower-case "_tready". This
# is only true for ports inside interfaces; in general, the case must
# match whatever you have in your Verilog
inputs_thd.set_signal("new_tready", "1")
# Write a few flits with no byte swapping
inputs_thd.set_signal("enable_swap", "0")
old.write(inputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
old.write(inputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
# Write a few flits with byte swapping
inputs_thd.wait_negedge("clk")
inputs_thd.set_signal("enable_swap", "1")
old.write(inputs_thd, "0x01234567", tkeep="0xF", tlast="0")
old.write(inputs_thd, "0x89ABCDEF", tkeep="0xC", tlast="1")
# Finally, signal that we're finished by setting flag 0
inputs_thd.set_flag(0)

# OUTPUT THREAD
# Read flits and state our expectations
new.read(outputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
new.read(outputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
new.read(outputs_thd, "0x67452301", tkeep="0xF", tlast="0")
new.read(outputs_thd, "0xEFCDAB89", tkeep="0x3", tlast="1")
# Wait for flag 0 to make sure the input thread is finsihed
outputs_thd.wait_flag(0)
# Finally, end the simulation
outputs_thd.end_vector()

# TIMEOUT THREAD
timeout_thd = Thread()
timeout_thd.add_delay("5000ns")
timeout_thd.display("Simulation timed out!")
timeout_thd.end_vector()

# Add these threads to the TestVector
tv.add_thread(inputs_thd)
tv.add_thread(outputs_thd)
tv.add_thread(timeout_thd)

# Construct the Testbench object
tb = Testbench.default("axis_swap_endian")
tb.add_module(dut)
tb.add_test_vector(tv)

# Generate the SystemVerilog testbench
tb.generateTB(os.getcwd() + "/", "sv")
