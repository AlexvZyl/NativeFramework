# This module generates a script to be executed inside Lumen.
# It acts as a wrapper for the Lua functionality.

# ------------- #
#  S C R I P T  #
# ------------- #

def BeginScript():
    global __script
    __script = ""

def EndScript():
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

# ------------------- #
#  U T I L I T I E S  #
# ------------------- #

# Return and clear the generated script.
def GetScript():
    global __script
    copy = __script
    __script = ""
    return copy

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
        line += p + ","
    line = line[:-1] + ")"
    __AddLine(line)

# ------- #
#  E O F  #
# ------- #


