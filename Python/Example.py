# Imports.
from Lumen import LumenScript, LumenInstance

# Create Lumen program instance.
instance = LumenInstance("C:\\Lumen\\Binaries\\Executables\\x64\\Release\\Lumen.exe")

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

print(script.Get())

# Execute the first script and clear so that we can use it again.
instance.ExecuteScript(script)
script.Clear()

# Write second script.
script.BeginScene2D("Another Test")
script.DrawQuad2D((2,2,2), (3,3,3), (1,0,0,1))
script.DrawLine2D((2,1,4), (2,2,1), 0.1, (1,0,0,1))

# Execute second script and clear it.
instance.ExecuteScript(script)
script.Clear()

# Stop instance (this will close Lumen).
instance.Shutdown()
