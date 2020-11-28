#!/usr/bin/python2

import numpy as np

class exampleContent:
   pass

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

   # Prune away all the nodes from this node that is not nearest to the target value
   def prune (self, targetValue):
      if self.edge: return self.value
      values = []
      for node in self.nodes:
         values.append (node.prune (targetValue))
      values = np.array (values)
      minInd = np.argmin (np.abs (values - targetValue))
      self.nodes = self.nodes[minInd]
      self.value = self.nodes.value
      return self.value

   # Prune away all the nodes from this node that are not a path to the max target value
   def maxPrune (self):
      if self.edge: return self.value
      maxValue = float ("-inf")
      values = []
      for node in self.nodes:
         values.append (node.maxPrune ())
      values = np.array (values)
      ind = np.argmax (values)
      self.nodes = [self.nodes[ind]]
      self.value = self.nodes[0].value
      return self.value

   def pruneKeepMin (self):
      for node in self.nodes:
         node.pruneKeepMin ()

      if not self.edge: return

      minValue = float ("inf")
      minNode = None
      for node in self.nodes:
         if node.value < minValue:
            minValue = node.value
            minNode = node
      if minNode is not None:
         self.nodes = [minNode]

   def __str__ (self):
      contents = self.depth * " " + str (self.value) + "\n"
      for node in self.nodes:
         contents += node.__str__ ()
      return contents

# Example
content = exampleContent

tree = Node (0, content, 0.0)
node1 = tree.addNode (content, 1.0)
node2 = node1.addNode (content, 2.0)
node3 = node2.addNode (content, 3.0)

node1.addNode (content, 2.1)

print (tree)
