from Lumen import *

BeginScript()

BeginScene2D("Testing Scene")
DrawQuad2D((0,0,0), (1,1,1), (0,0,0,1))
DrawLine2D((0,0,0), (1,1,1), 0.1, (0,0,0,1))
print(GetScript())

ClearScript()


