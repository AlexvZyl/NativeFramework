# This module generates a script to be executed inside Lumen.
# It acts as a wrapper for the Lua functionality.

# --------------- #
#  I M P O R T S  #
# --------------- #

import asyncio
from subprocess import PIPE, Popen
from threading import Thread, local
from queue import Queue, Empty
import sys
import websocket
import websockets

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

# --------------------------------------- #
#  L U M E N   S C R I P T   E N T I T Y  #
# --------------------------------------- #

class _LumenScriptEntity:

    def __init__(self):
        self._script = ""
        self._newLine = "\n"

    # Return the generated script.
    def GetLua(self):
        return self._script

    # Add a line to the script.
    def _AddLine(self, line):
        self._script += line + self._newLine

    # Add a function with the parameters to the script.
    def _AddFunction(self, function, parameters):
        line = function + "("
        for p in parameters:
            line += str(p) + ", "
        line = line[:-2] + ")"
        self._AddLine(line)

# ------------------------- #
#  L U M E N   S C R I P T  #
# ------------------------- #

class LumenScript(_LumenScriptEntity):

    def __init__(self):
        _LumenScriptEntity.__init__(self)
        self._AddLine("-- LUA_EXECUTABLE_SCRIPT")

    def Clear(self):
        self._script = ""
        self._AddLine("-- LUA_EXECUTABLE_SCRIPT")

    # ------------- #
    #  S C E N E S  #
    # ------------- #

    def BeginScene2D(self, name):
        self._AddFunction("BeginScene2D", (PString(name),))

    # --------------------- #
    #  P R I M I T I V E S  #
    # --------------------- #

    def DrawQuad2D(self, vertex1, vertex2, color):
        self._AddFunction("DrawQuad2D", (PTable(vertex1), PTable(vertex2), PTable(color)))

    def DrawRotatedQuad2D(self, vertex1, vertex2, color, degrees):
        self._AddFunction("DrawRotatedQuad2D", (PTable(vertex1), PTable(vertex2), PTable(color), degrees))

    def DrawLine2D(self, vertex1, vertex2, thickness, color):
        self._AddFunction("DrawLine2D", (PTable(vertex1), PTable(vertex2), thickness, PTable(color)))

    def DrawText2D(self, text, position, color, scale, horizontalAlignment, verticalAlignment):
        self._AddFunction("DrawText2D", (PString(text), PTable(position), PTable(color), scale, PString(horizontalAlignment), PString(verticalAlignment)))

    def DrawRotatedText2D(self, text, position, color, scale, horizontalAlignment, verticalAlignment, degrees):
        self._AddFunction("DrawRotatedText2D", (PString(text), PTable(position), PTable(color), scale, PString(horizontalAlignment), PString(verticalAlignment), degrees))

    def DrawCircle2D(self, center, radius, color, thickness, fade):
        self._AddFunction("DrawCircle2D", (PTable(center), radius, PTable(color), thickness, fade))

    def DrawTriangle2D(self, vertex1, vertex2, vertex3, color):
        self._AddFunction("DrawTriangle2D", (PTable(vertex1), PTable(vertex2), PTable(vertex3), PTable(color)))

# ------------------- #
#  L U M E N   G U I  #
# ------------------- #

class LumenGui(_LumenScriptEntity):

    def __init__(self, guiName):

        _LumenScriptEntity.__init__(self)
        self._AddLine("-- LUA_SCRIPT_GUI")
        self._AddLine("-- Gui Name: '" + guiName + "'.")
        # Used to set the server message callback function.
        self.ServerHandler = 0

       

    def StartServer(self, LumenInstance):

        # Start server.
        self.host = "127.0.0.1"
        self.port = 0
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
        start_server = websockets.serve(self.LumenGuiServer, self.host, self.port, ping_interval=None)
        asyncio.get_event_loop().run_until_complete(start_server)

        # We provide the port as 0, so the OS gives an open port.
        # This is the only way I could find the port that it connects to.
        # It is VERY hacky, since the API does not allow for this.
        laddrString =  str(start_server.ws_server.sockets[0])
        index = laddrString.index("laddr")
        laddrString = laddrString[index:]
        laddrString = laddrString.replace("laddr=('", "")
        laddrString = laddrString.replace(str(self.host), "")
        laddrString = laddrString.replace("', ", "")
        laddrString = laddrString.replace(")>", "")
        self.port = laddrString
        
        # Write url to script.
        self._AddLine("-- Websocket: '" + self.host + ":" + self.port + "'.")
        LumenInstance.ExecuteScript(self)
        asyncio.get_event_loop().run_forever()
        
    async def LumenGuiServer(self, websocket, path):
        async for message in websocket:
            self.ServerHandler(message)

    # ----------------------------- #
    #  G U I   C O M P O N E N T S  #
    # ----------------------------- #

    def Button(self, label, size):
        self._AddFunction("ImGui_Button", (PString(label), PTable(size)))

    def Text(self, text):
        self._AddFunction("ImGui_Text", (PString(text),))

# ---------------------------- #
#  L U M E N   I N ST A N C E  #
# ---------------------------- #

class LumenInstance:

    def __init__(self, executablePath):

        self._executablePath = executablePath
        self._running = True

        # Non blocking read setup.
        ON_POSIX = 'posix' in sys.builtin_module_names
        def enqueue_output(out, queue):
            while self._running:
                for line in iter(out.readline, b''):
                    queue.put(line)
                    break

        # Start Lumen & thread for reading.
        self._process = Popen(self._executablePath, stdout = PIPE, text = True, close_fds=ON_POSIX)
        q = Queue()
        t = Thread(target=enqueue_output, args=(self._process.stdout, q))
        t.start()

        # Find socket output from Lumen.
        lumenSocketIDString = "[Lumen] [Websocket] : Connected to '"
        line = ""
        search = True
        while(search):
            try:    line = q.get()
            except  Empty: pass
            else:
                if(line.find(lumenSocketIDString) != -1):
                    search = False

        # For some reason these wont close? FIX!!
        # self._running = False
        # q.join()
        # t.join()

        # Find and connect to port.
        url = line.replace(lumenSocketIDString, "")
        url = url.replace("'.\n", "")
        self._webSocket = websocket.create_connection(url)

    def ExecuteScript(self, script):
        self._webSocket.send(script.GetLua())

    def Shutdown(self):
        self._webSocket.close()
        self._process = None

# ------- #
#  E O F  #
# ------- #