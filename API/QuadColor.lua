BeginScene2D("TestingCircuit")

function table.table_copy(t)
   local t2 = {}
   for k,v in pairs(t) do
      t2[k] = v
   end
   return t2
end

quadWidth = 0.2
quadHeight = 0.2
xInc = quadWidth
yInc = quadHeight
vertex1 = {-4,-2,0}
vertex2 = {vertex1[1] + quadWidth, vertex1[2] + quadHeight, vertex1[3]}

for i = 0, 40, 1 do
  newVertex1 = table.table_copy(vertex1)
  newVertex1[1] = newVertex1[1] + (xInc * i)
  newVertex2 = table.table_copy(vertex2)
  newVertex2[1] = newVertex2[1] + (xInc * i)
  for j = 0, 20, 1 do 
    r = (i+1) / 50
    g = (j+1) / 50
    b = (i + j + 1) / 40
    a = 1t
    newVertex1[2] = newVertex1[2] + yInc
    newVertex2[2] = newVertex2[2] + yInc
    DrawQuad2D(newVertex1, newVertex2, {r, g, b, a})
  end
end