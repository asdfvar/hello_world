#!/usr/bin/python2

class Node:
   def __init__ (self, depth, content, value):
      self.edge = True
      self.contents = content
      self.value = value
      self.nodes = []
      self.depth = depth

   def addNode (self, content, value):
      newNode = Node (self.depth+1, content, value)
      self.nodes.append (newNode)
      self.edge = False
      return newNode

   def pruneKeepMin (self):
      for node in self.nodes:
         node.pruneKeepMin ()

      minValue = float ("inf")
      minNode = None
      for node in self.nodes:
         if node.value < minValue:
            minValue = node.value
            minNode = node
      if minNode is not None:
         self.nodes = [minNode]

   def pruneKeepMax (self):
      for node in self.nodes:
         node.pruneKeepMin ()

      maxValue = float ("-inf")
      maxNode = None
      for node in self.nodes:
         if node.value > maxValue:
            maxValue = node.value
            maxNode = node
      if maxNode is not None:
         self.nodes = [maxNode]

   def __str__ (self):
      contents = self.depth * " " + str (self.value) + "\n"
      for node in self.nodes:
         contents += node.__str__ ()
      return contents

# Example
if __name__ == "__main__":
   class exampleContent: pass
   content = exampleContent

   node0 = Node (0, content, 0.0)
   node1 = node0.addNode (content, 1.0)
   node2 = node1.addNode (content, 2.0)
   node3 = node2.addNode (content, 3.0)

   node0.addNode (content, 1.1)
   node1.addNode (content, 2.1)
   node3.addNode (content, 4.1)
   node3.addNode (content, 4.2)
   node3.addNode (content, 4.3)

   print ("before pruning:")
   print (node0)

   print ("after pruning:")
   node0.pruneKeepMin ()
   print (node0)
