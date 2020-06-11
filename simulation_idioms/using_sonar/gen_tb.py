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
# Write a few flits with no byte swapping
inputs_thd.set_signal("enable_swap", "0")
old.write(inputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
old.write(inputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
# Write a few flits with byte swapping
inputs_thd.wait_negedge("clk")
inputs_thd.set_signal("enable_swap", "1")
old.write(inputs_thd, "0x01234567", tkeep="0xF", tlast="0")
old.write(inputs_thd, "0x89ABCDEF", tkeep="0xC", tlast="1")

# OUTPUT THREAD
# Read flits and state our expectations
new.read(outputs_thd, "0xDEADBEEF", tkeep="0xF", tlast="0")
new.read(outputs_thd, "0xCAFEBABE", tkeep="0xC", tlast="1")
new.read(outputs_thd, "0x67452301", tkeep="0xF", tlast="0")
new.read(outputs_thd, "0xEFCDAB89", tkeep="0x3", tlast="1")

# Add these two threads to the TestVector
tv.add_thread(inputs_thd)
tv.add_thread(outputs_thd)

# Construct the Testbench object
tb = Testbench.default("axis_swap_endian")
tb.add_module(dut)
tb.add_test_vector(tv)

# Generate the SystemVerilog testbench
tb.generateTB(os.getcwd(), "sv")
