# This module generates a script to be executed inside Lumen.
# It acts as a wrapper for the Lua functionality.

# --------------- #
#  I M P O R T S  #
# --------------- #

from subprocess import PIPE, Popen
from threading import Thread
from queue import Queue, Empty
import sys
import websocket

# --------------------------------------- #
#  P A R A M E T E R   F U N C T I O N S  #
# --------------------------------------- #

# Turn the parameter into a string.
def PString(parameter):
    return "\"" + parameter + "\""

# Turn the parameter into a Lua table.
def  PTable(parameters):
    string = "{"
    for p in parameters:
        string += str(p) + ","
    return string[:-1] + "}"


# ------------------------- #
#  L U M E N   S C R I P T  #
# ------------------------- #

class LumenScript:

    def __init__(self):
        self.__script = ""
        self.__newLine = "\n"

    # Clear the script.
    def Clear(self):
        self.__script = ""

    # Return the generated script.
    def Get(self):
        return self.__script

    # ------------- #
    #  S C E N E S  #
    # ------------- #

    def BeginScene2D(self, name):
        self.__AddFunction("BeginScene2D", (PString(name),))

    # --------------------- #
    #  P R I M I T I V E S  #
    # --------------------- #

    def DrawQuad2D(self, vertex1, vertex2, color):
        self.__AddFunction("DrawQuad2D", (PTable(vertex1), PTable(vertex2), PTable(color)))

    def DrawRotatedQuad2D(self, vertex1, vertex2, color, degrees):
        self.__AddFunction("DrawRotatedQuad2D", (PTable(vertex1), PTable(vertex2), PTable(color), degrees))

    def DrawLine2D(self, vertex1, vertex2, thickness, color):
        self.__AddFunction("DrawLine2D", (PTable(vertex1), PTable(vertex2), thickness, PTable(color)))

    def DrawText2D(self, text, position, color, scale, horizontalAlignment, verticalAlignment):
        self.__AddFunction("DrawText2D", (PString(text), PTable(position), PTable(color), scale, PString(horizontalAlignment), PString(verticalAlignment)))

    def DrawRotatedText2D(self, text, position, color, scale, horizontalAlignment, verticalAlignment, degrees):
        self.__AddFunction("DrawRotatedText2D", (PString(text), PTable(position), PTable(color), scale, PString(horizontalAlignment), PString(verticalAlignment), degrees))

    def DrawCircle2D(self, center, radius, color, thickness, fade):
        self.__AddFunction("DrawCircle2D", (PTable(center), radius, PTable(color), thickness, fade))

    def DrawTriangle2D(self, vertex1, vertex2, vertex3, color):
        self.__AddFunction("DrawTriangle2D", (PTable(vertex1), PTable(vertex2), PTable(vertex3), PTable(color)))

    # --------------- # 
    #  P R I V A T E  #
    # --------------- # 

    # Add a line to the script.
    def __AddLine(self, line):
        self.__script += line + self.__newLine

    # Add a function with the parameters to the script.
    def __AddFunction(self, function, parameters):
        line = function + "("
        for p in parameters:
            line += str(p) + ", "
        line = line[:-2] + ")"
        self.__AddLine(line)

# ---------------------------- #
#  L U M E N   I N ST A N C E  #
# ---------------------------- #

class LumenInstance:

    def __init__(self, executablePath):

        # Set the path.
        self.__executablePath = executablePath
        self.__running = True

        # Non blocking read setup.
        ON_POSIX = 'posix' in sys.builtin_module_names
        def enqueue_output(out, queue):
            while self.__running:
                for line in iter(out.readline, b''):
                    queue.put(line)
                    break

        # Start Lumen & thread.
        self.__process = Popen(self.__executablePath, stdout = PIPE, text = True, close_fds=ON_POSIX)
        q = Queue()
        t = Thread(target=enqueue_output, args=(self.__process.stdout, q))
        t.start()

        # Find socket output from Lumen.
        lumenSocketIDString = "[LUMEN] [WEBSOCKET] : Connected to '"
        line = ""
        search = True
        while(search):
            try:    line = q.get()
            except  Empty: pass
            else:
                # Check if Socket ID is found.
                if(line.find(lumenSocketIDString) != -1):
                    search = False

        # For some reason these wont close?
        # FIX!!
        # self.__running = False
        # q.join()
        # t.join()

        # Find the port Lumen connected to in the string.
        # Hard coded for now, but this will be contained in 'line'.
        url = line.replace(lumenSocketIDString, "")
        url = url.replace("'.\n", "")

        # Connect to socket.
        self.__webSocket = websocket.create_connection(url)

    def ExecuteScript(self, script):
        self.__webSocket.send(script.Get())

    def Shutdown(self):
        self.__webSocket.close()
        self.__process = None

# ------- #
#  E O F  #
# ------- #