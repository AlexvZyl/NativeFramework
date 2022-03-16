# Imports.
from Lumen import LumenScript, LumenInstance

# Create Lumen program instance.
instance = LumenInstance("C:\\Lumen\\Binaries\\Executables\\x64\\Release\\Lumen.exe")

# Create a script.
script = LumenScript()

# Write first script.
script.BeginScene2D("Test")
script.DrawQuad2D((0,0,0), (1,1,1), (0,0,0,1))
script.DrawQuad2D((2,2,2), (3,3,3), (1,0,0,1))
script.DrawLine2D((2,1,4), (2,2,1), 0.1, (1,0,0,1))

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