# Imports.
from Lumen import LumenScript, LumenInstance, LumenGui
import threading

# Create Lumen program instance.
# instance = LumenInstance("C:\\Lumen\\Binaries\\Executables\\x64\\Release\\Lumen.exe")
instance = LumenInstance("C:\\Lumen\\Binaries\\Executables\\x64\\Debug\\Lumen.exe")

# Create a script.
script = LumenScript()

# Write first script.
script.BeginScene2D("Test")
script.DrawQuad2D((0,0,0), (1,1,0), (1,0,0,1))
lineThickness = 0.01
script.DrawLine2D((0-lineThickness,0,0), (0-lineThickness,1,-0), lineThickness, (1,0,1,1))
script.DrawRotatedQuad2D((0,0,0.98), (1,1,0.98), (1,1,0,0.6), 45)
script.DrawText2D("Testing Text (L, B)", (0,0,0.99), (0,0,0,1), 0.1, "L", "B")
script.DrawRotatedText2D("Testing Rotated Text (C, B)", (0.5,0.5,0.99), (0,0,0,1), 0.05, "C", "B", 30)
script.DrawCircle2D((0.5,0.5,0.999), 0.1, (0,1,1,0.6), 1, 0.001)
script.DrawTriangle2D((1,1,0.99), (1,0,0.99), (1.4, 0.5, 0.99), (1,1,1,0.7))
print(script.GetLua())

# Execute the first script and clear so that we can use it again.
instance.ExecuteScript(script)
script.Clear()

# Write second script.
script.BeginScene2D("Another Test")
script.DrawQuad2D((2,2,2), (3,3,3), (1,0,0,1))
script.DrawLine2D((2,1,4), (2,2,1), 0.1, (1,0,0,1))
print(script.GetLua())

# Execute second script and clear it.
instance.ExecuteScript(script)
script.Clear()

# Create a Lumen Gui.
imgui = LumenGui("Testing Gui")

# Setup the server handler.
def MyServerHandler(message):
    print("MyServerHandler: " + message)
    # Handle the message...

    # Work in progress.
    # Use this to clear and write a new script.
    # if(True):
    #     imgui.Clear()
    #     imgui.Text("This is a new GUI.")
    #     print(imgui.GetLua())
    #     imgui.NewScript()

# Set the specific gui's handler.
imgui.ServerHandler = MyServerHandler

# Create gui script.
imgui.Separator()

imgui.Text("Testing Text")
imgui.Button("Testing Button", (0,0))
imgui.Text("Some more text.")
imgui.Button("Sized button", (200,0))

imgui.Separator()

imgui.Text("Text before button:")
imgui.SameLine(0)
imgui.Button("Sameline button", (0,0))

imgui.Separator()

# If you make the last parameter -1 imgui wil display all of the entries.
imgui.Combo("ComboboxTest", 0, ("Test0", "Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7"), 4) 

imgui.Separator()

imgui.Checkbox("Checkbox 1 True", True)
imgui.Checkbox("Checkbox 2 False", False)

imgui.Separator()
    
imgui.InputText("Input Text 1", "Initial Entry")
imgui.InputText("Input Text 2", "")

imgui.Separator()

dictionary = {
    "SingleKey" : "SingleValue",
    1           : 2,
    "Key1"      : ["Value1", "Value2", "Value3"],
    "Key2"      : [1, 2, 3],
    3           : ["V1", 2, 3],
    4           : [1, "V2", 3]
}

imgui.Table("Table Label", 150, dictionary)

imgui.Separator()

imgui.CloseButton("Submit", (100, 30))

# Work in progress.
imgui.ClearAwaitScriptButton("Clear Await", (100, 30))

imgui.Separator()

# Start gui inside the Lumen instance.
threading.Thread(imgui.StartServer(instance))

# Stop instance (this will close Lumen).
# instance.Shutdown()  