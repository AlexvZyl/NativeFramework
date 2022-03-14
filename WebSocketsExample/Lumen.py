# This module generates a script to be executed inside Lumen.
# It acts as a wrapper for the Lua functionality.

# ------------- #
#  S C R I P T  #
# ------------- #

def BeginScript():
    global __script
    __script = ""

def ClearScript():
    global __script
    __script = ""

# ------------- #
#  S C E N E S  #
# ------------- #

def BeginScene2D(name):
    __AddFunction("BeginScene2D", (__String(name),))

# --------------------- #
#  P R I M I T I V E S  #
# --------------------- #

def DrawQuad2D(vertex1, vertex2, color):
    __AddFunction("DrawQuad2D", (__Table(vertex1), __Table(vertex2), __Table(color)))

def DrawLine2D(vertex1, vertex2, thickness, color):
    __AddFunction("DrawLine2D", (__Table(vertex1), __Table(vertex2), thickness, __Table(color)))

# ------------------- #
#  U T I L I T I E S  #
# ------------------- #

# Return the generated script.
def GetScript():
    global __script
    return __script

# --------------- #
#  P R I V A T E  #
# --------------- #

# The script that the function write to.
__script = ""
# New line character.
__newLine = "\n"

# Turn the parameter into a string.
def __String(parameter):
    return "\"" + parameter + "\""

# Turn the parameter into a Lua table.
def  __Table(parameters):
    string = "{"
    for p in parameters:
        string += str(p) + ","
    return string[:-1] + "}"

# Add a line to the script.
def __AddLine(line):
    global __script
    __script += line + __newLine

# Add a function with the parameters to the script.
def __AddFunction(function, parameters):
    line = function + "("
    for p in parameters:
        line += str(p) + ", "
    line = line[:-2] + ")"
    __AddLine(line)

# ------- #
#  E O F  #
# ------- #


