import LumenLua as LL

LL.BeginScript()

LL.BeginScene2D("Testing Scene")
LL.DrawQuad2D((0,0,0), (1,1,1), (0,0,0,1))
print(LL.GetScript())

LL.EndScript()