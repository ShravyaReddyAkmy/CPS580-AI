
#This is the only file you need to work on. You do NOT need to modify other files

# Below are the functions you need to implement. For the first project, you only need to finish implementing bfs() and dfs()

from game.Node import Node
from copy import deepcopy
from collections import deque
from queue import PriorityQueue
import time

# Pre-defined variables for final state of puzzle 
# and child states for each node
goalstate=[0,1,2,3,4,5,6,7,8]
childrenList={
        0:[1,3],
        1:[0,2,4],
        2:[1,5],
        3:[0,4,6],
        4:[1,3,5,7],
        5:[2,4,8],
        6:[3,7],
        7:[4,6,8],
        8:[5,7]
        }

# Function to back trace the order of moves 
# after attaining final goalstate.
def pathtrace(node):
    path=[]
    path.append(node.puzzle.index(8))
    parentNode=node.parent
    while parentNode != None:
        path.append(parentNode.puzzle.index(8))
        parentNode=parentNode.parent
    print('GoalState achieved. Moves =',len(path)-1)
    path.reverse()
    return path


#here you need to implement the Breadth First Search Method
def bfs(puzzle):
    list = []
    visited = []
    initNode=Node(None,puzzle,0)
    initNode.children=childrenList[initNode.puzzle.index(8)]
    
    q=deque([initNode])
    startTime=time.time()
    while q:
        popstate=q.popleft()
        loc8=popstate.puzzle.index(8)

        if popstate.puzzle == goalstate:
            list=pathtrace(popstate)
            # print('Path found \nNo. of moves =',len(list)-1)
            endTime=time.time()
            print('Time taken for BFS =',endTime-startTime)
            return list
            
        if popstate.puzzle not in visited:
            visited.append(popstate.puzzle)

            children=popstate.children
            for child in children:
                childNode=Node(popstate,None,len(visited))
                currstate=deepcopy(popstate)
                currstate.puzzle[loc8],currstate.puzzle[child]=currstate.puzzle[child],currstate.puzzle[loc8]
                childNode.puzzle=currstate.puzzle
                childNode.children=childrenList[childNode.puzzle.index(8)]

                if childNode.puzzle == goalstate:
                    list=pathtrace(childNode)
                    # print('Path found.\nNo. of moves =',len(list)-1)
                    endTime=time.time()
                    print('Time taken for BFS =',endTime-startTime)
                    return list               
                else:
                    q.append(childNode)
    

#here you need to implement the Depth First Search Method
def dfs(puzzle):
    list = []
    visited = []
    initNode=Node(None,puzzle,0)
    initNode.children=childrenList[initNode.puzzle.index(8)]
    
    s=deque([initNode])
    startTime=time.time()
    while s:
        popstate=s.pop()
        loc8=popstate.puzzle.index(8)

        if popstate.puzzle == goalstate:
            list=pathtrace(popstate)
            # print('Path found 1.\nNo. of moves=',len(list)-1)
            endTime=time.time()
            print('Time taken for DFS =',endTime-startTime)
            return list
            
        if popstate.puzzle not in visited:
            visited.append(popstate.puzzle)

            children=popstate.children
            for child in children:
                childNode=Node(popstate,None,len(visited))
                currstate=deepcopy(popstate)
                currstate.puzzle[loc8],currstate.puzzle[child]=currstate.puzzle[child],currstate.puzzle[loc8]
                childNode.puzzle=currstate.puzzle
                childNode.children=childrenList[childNode.puzzle.index(8)]

                if childNode.puzzle == goalstate:
                    list=pathtrace(childNode)
                    # print('Path found.\nno. of moves =',len(list)-1)
                    endTime=time.time()
                    print('Time taken for DFS =',endTime-startTime)
                    return list               
                else:
                    s.append(childNode)


# Project-2 A-star implementation
from itertools import count
unique = count()

def astar(puzzle):
    list = []
    expanded = []

    #Function for future cost from current child nodes to goal node
    def heuristic(node):
        h=0
        for i in node.puzzle:
            h+=abs(goalstate.index(i)-node.puzzle.index(i))
        return h

    #Function for actual cost from current nodes to child node
    def actualcost(parentcost):
        g=parentcost+1
        return g

    initNode=Node(None,puzzle,0)
    initNode.children=childrenList[initNode.puzzle.index(8)]
    initNode.h=heuristic(initNode)
    initNode.f=initNode.cost+initNode.h
    
    pq=PriorityQueue()
    pq.put((initNode.f,next(unique),initNode))

    startTime=time.time()
    while not pq.empty():
        getstate=pq.get()[2]
        loc8=getstate.puzzle.index(8)

        if getstate.puzzle == goalstate:
            list=pathtrace(getstate)
            endTime=time.time()
            print('Time taken for A-star =',endTime-startTime)
            return list

        expanded.append(getstate.puzzle)
        children=getstate.children
        for child in children:
            childNode=Node(getstate,None,actualcost(getstate.cost))
            currstate=deepcopy(getstate)
            currstate.puzzle[loc8],currstate.puzzle[child]=currstate.puzzle[child],currstate.puzzle[loc8]
            childNode.puzzle=currstate.puzzle
            childNode.children=childrenList[childNode.puzzle.index(8)]

            childNode.h=heuristic(childNode)
            childNode.f=childNode.cost+childNode.h

            # print(childNode.cost,childNode.h,childNode.f,childNode.puzzle)
            if childNode.puzzle not in expanded:
                pq.put((childNode.f,next(unique),childNode))



