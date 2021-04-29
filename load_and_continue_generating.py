import random
import time
import pygame
import numpy as np
from copy import *
from sys import exit

import pickle
import time

def drawpoly(poly,xalign,yalign,size,mult,colour=(255,0,0)):
    drawpos = deepcopy(poly)
    drawpos[0][0] *=mult
    drawpos[0][1] *=mult
    drawpos[0][0] += xalign   
    drawpos[0][1] += yalign
    drawpos[0][0] += 30   
    drawpos[0][1] += 30
    drawpos[1][0] *=mult
    drawpos[1][1] *=mult
    drawpos[1][0] += xalign   
    drawpos[1][1] += yalign
    drawpos[1][0] += 30   
    drawpos[1][1] += 30
    drawpos[2][0] *=mult
    drawpos[2][1] *=mult
    drawpos[2][0] += xalign   
    drawpos[2][1] += yalign
    drawpos[2][0] += 30   
    drawpos[2][1] += 30
    
    for i in range(0,size):
        for j in range(0,size):
            pygame.draw.rect(screen,(0,0,0),(xalign+30+mult*i, yalign+30+mult*j, 1, 1))

    pygame.draw.polygon(screen,(0,0,0),([xalign+30,yalign+30],[xalign+mult*size+30,yalign+30],[xalign+size*mult+30,yalign+size*mult+30],[xalign+30,yalign+size*mult+30]),2)
    pygame.draw.polygon(screen,colour,drawpos,3)


# calculate edge lengths, throw out colinear sets
def calc_distance(poly):
    d1 = (abs(poly[0][0]-poly[1][0])**2 + abs(poly[0][1]-poly[1][1])**2)**0.5
    d2 = (abs(poly[0][0]-poly[2][0])**2 + abs(poly[0][1]-poly[2][1])**2)**0.5
    d3 = (abs(poly[2][0]-poly[1][0])**2 + abs(poly[2][1]-poly[1][1])**2)**0.5

    if abs(2*max([d1,d2,d3]) - sum([d1,d2,d3])) < 0.0000000001:
        return{0}
    else:
        a1 = np.arccos((d1**2 + d2**2 - d3**2)/(2*d1*d2))
        a2 = np.arccos((d2**2 + d3**2 - d1**2)/(2*d2*d3))
        a3 = np.arccos((d3**2 + d1**2 - d2**2)/(2*d3*d1))
        #return {a1,a2,a3}
        return {d1,d2,d3}


def pointsinside(size,p):
    v_list = deepcopy(p)
    for x in range(0,size+1):
        for y in range(0,size+1):
            A = np.array([p[0]])
            B = np.array([p[1]])
            C = np.array([p[2]])
            P = np.array([x,y])
            AB = A-B
            BC = B-C
            CA = C-A
            AP = A-P
            BP = B-P
            CP = C-P
            p1 = np.cross(AB,AP)
            p2 = np.cross(BC,BP)
            p3 = np.cross(CA,CP)
            if (p1 >= 0 and p2 >= 0 and p3 >= 0) or (p1 <= 0 and p2 <= 0 and p3 <= 0):
                if [x,y] not in v_list:
                    v_list.append([x,y])
    return v_list

def pointsinside_noboundary(size,p):
    v_list = []
    for x in range(0,size+1):
        for y in range(0,size+1):
            A = np.array([p[0]])
            B = np.array([p[1]])
            C = np.array([p[2]])
            P = np.array([x,y])
            AB = A-B
            BC = B-C
            CA = C-A
            AP = A-P
            BP = B-P
            CP = C-P
            p1 = np.cross(AB,AP)
            p2 = np.cross(BC,BP)
            p3 = np.cross(CA,CP)
            if (p1 > 0 and p2 > 0 and p3 > 0) or (p1 < 0 and p2 < 0 and p3 < 0):
                v_list.append([x,y])
    return v_list

def affine_eq(t,comp,verbose=False):
    #loop over all equiareal maps
    permutations = [(1,2,0),(2,0,1),(0,2,1),(1,0,2),(2,1,0),(0,1,2)]

    for i in permutations:
        if verbose:
            print(i)
        A = [[0,0,0],[0,0,0],[1,1,1]]
        A[0] = [t[i[0]][0],t[i[1]][0],t[i[2]][0]]
        A[1] = [t[i[0]][1],t[i[1]][1],t[i[2]][1]]
        A = np.array(A)
        unique = 1
        if t != comp:

            X = [[0,0,0],[0,0,0],[1,1,1]]
            #convert 2-tuples into 3-tuples
            X[0] = [comp[0][0],comp[1][0],comp[2][0]]
            X[1] = [comp[0][1],comp[1][1],comp[2][1]]
            X = np.array(X)

            T = X.dot(np.linalg.inv(A))

            if verbose:
                #print(A)
                #print(X)
                print("T and det(T)")
                print(T)
                print(np.linalg.det(T))
                print("integer checks:")

            if np.array_equal(A,X):
                return True

            det = np.linalg.det(T)

            if abs(det-1) <= 0.0000001 or abs(det+1) <= 0.0000001:
                in_z = 1
                for i in T:
                    for j in i:
                        if verbose:
                            print(j)
                        if abs(int(j) - j) > 0.0000001:
                            if abs(int(j+0.000002) - j) > 0.0000001:
                                if abs(int(j-0.000002) - j) > 0.0000001:
                                    if verbose:
                                        print("failed with " +str(abs(int(j+0.00000002) - j)))
                                    in_z = 0
                                    break


                if in_z:
                    return [True,T]
        else:
            return True
    return [False,T]

def A_smaller_than_B(A,B):
    A_max = 0
    for i in A:
        for j in i:
            if j>A_max:
                A_max = j
    B_max = 0
    for i in B:
        for j in i:
            if j>B_max:
                B_max = j
    return A_max < B_max

pygame.display.init()

screen = pygame.display.set_mode((1500,1000))
screen.fill((255,255,255))
pygame.font.init()
font = pygame.font.Font(None,32)

clock = pygame.time.Clock()
FRAMES_PER_SECOND = 30
down = False

load = True
xalign = 0
yalign = 0
start_size = 23
size_max = 23
data=open("generated_verticies.pkl","rb")
mult = 3
pad = 70
printno = 300
columns = 20


#generate triangles of size (size)
if load:
    smallpolygons = pickle.load(data)
    data.close()
    for i in range(0,len(smallpolygons)):
        smallpolygons[i] = smallpolygons[i][0]
    data=open("small.pkl","rb")
    extra = pickle.load(data)
    affine_copy = []
    data.close()
    for p in extra:
        smallpolygons.append(p)

    maxsize = 0
    for p in smallpolygons:
        for j in p:
            for i in j:
                if i > maxsize:
                    maxsize = i
    print(str(len(smallpolygons))+" below size " + str(maxsize)+" polygons loaded")
    start_size = maxsize
else:
    smallpolygons = []
    affine_copy = []
if size_max == start_size:
    print("no size increase")

oldsize = 0
timer = time.perf_counter()
for size in range(start_size+1,size_max+1):
    print("time elapsed:" + str(round(time.perf_counter()-timer,3)))
    timer = time.perf_counter()
    print(str(size)+":"+str(len(smallpolygons)-oldsize))
    oldsize =  len(smallpolygons)
    print("smaller polygons:"+str(oldsize))
    distances = []
    triangles = []
    for n in range(0,size+1):
        for x in range(0,size+1):
            for y in range(0,size+1):
                if (x != 0 or y != 0):
                    newpoly = []
                    newpoly.append([0,0])
                    newpoly.append([size,n])
                    newpoly.append([x,y])
                    d = calc_distance(newpoly)
                    if 0 not in d:  #non colinear
                        distances.append(d)
                        triangles.append(newpoly)

    #print("generated base triangles")
                
    largelist = []
    affine_removed_small = deepcopy(smallpolygons)
    triangle_pos = len(affine_removed_small)-1
    for t in triangles:
        affine_removed_small.append(t)

    #bin sort polygons by total verticies
    vert_bins = {}
    int_bins = {}
    n=0
    print(str(len(affine_removed_small))+" polygons to sort")
    for p in affine_removed_small:
        v_list = pointsinside(size,p)
        if len(v_list) not in vert_bins.keys():
            vert_bins[len(v_list)] = {n}
        else:
            vert_bins[len(v_list)].add(n)
    
        v_list = pointsinside_noboundary(size,p)
        if len(v_list) not in int_bins.keys():
            int_bins[len(v_list)] = {n}
        else:
            int_bins[len(v_list)].add(n)
        n+=1
        if n%250 == 0:
            print(n)

    possible_classes = []
    for k in vert_bins.keys():
        for l in int_bins.keys():
            intersect = vert_bins[k].intersection(int_bins[l])
            if intersect != set():
                possible_classes.append(intersect)

    #print("possible_classes computed")
    affine_removed = []
    for pos_class in possible_classes:
        n=0
        short_comparison_list = []
        for i in pos_class:
            m=0
            unique = 1
            for j in short_comparison_list:
                if i != j:
                    m+=1
                    eq = affine_eq(affine_removed_small[i],affine_removed_small[j])
                    if eq == True or eq[0] == True:
                        if not A_smaller_than_B(affine_removed_small[i],affine_removed_small[j]):
                            unique = 0
                            break
                        else:
                            short_comparison_list.remove(j)
                            short_comparison_list.append(i)
                            unique = 0
                            break
            if unique:
                short_comparison_list.append(i)
            n+=1
            #print("made "+str(m)+" comparisons to "+str(i))
        for i in short_comparison_list:
            affine_removed.append(affine_removed_small[i])

    print(affine_removed)


    #print("affine equivalence with eachother complete")
        
    triangles_full_v = []


    #choose which to show
    if size == size_max:
        for i in affine_removed:
            if i in triangles:
                affine_copy.append([i[:],0])

    else:
        smallpolygons= deepcopy(affine_removed)
            #affine_copy.append([i[:],size_max-size])

print("time elapsed:" + str(round(time.perf_counter()-timer,3)))

small = []
for i in affine_copy:
    if i[1] > 0 and i[1] != 6:
        small.append(i[0])

gen = []
for i in affine_copy:
    if i[1] == 0:
        gen.append(i[0])


print(len(affine_copy))
#draw polygons
for poly_and_key in affine_copy:
    p=poly_and_key[0]
    if printno == 0:
        break
    else:
        printno -= 1

    #draw interior points
    v_list = pointsinside(size,p)
    for x in range(0,size+1):
        for y in range(0,size+1):
            if [x,y] in v_list:
                pygame.draw.rect(screen,(0,255,0),(xalign+30+mult*x-2, yalign+30+mult*y-2, 4,4))

    if poly_and_key[1] == 1:
        drawpoly(p,xalign,yalign,size,mult,(0,255,0))
    elif poly_and_key[1] == 2:
        drawpoly(p,xalign,yalign,size,mult,(0,0,255))
    elif poly_and_key[1] == 3:
        drawpoly(p,xalign,yalign,size,mult,(200,100,100))
    elif poly_and_key[1] == 4:
        drawpoly(p,xalign,yalign,size,mult,(0,255,255))
    elif poly_and_key[1] == 5:
        drawpoly(p,xalign,yalign,size,mult,(255,0,100))
    elif poly_and_key[1] == 6:
        drawpoly(p,xalign,yalign,size,mult,(0,255,255))
    elif poly_and_key[1] == 7:
        drawpoly(p,xalign,yalign,size,mult,(0,100,100))
    elif poly_and_key[1] == 8:
        drawpoly(p,xalign,yalign,size,mult,(100,150,50))
    elif poly_and_key[1] == 9:
        drawpoly(p,xalign,yalign,size,mult,(255,0,255))
    else:
        drawpoly(p,xalign,yalign,size,mult)

    triangles_full_v.append(v_list)
    xalign += pad
    xalign %= pad*columns
    if xalign == 0:
        yalign += pad

    
#for i in triangles_full_v:
#    print(i)

#save polygons

if size_max != start_size:
    savefile = open('generated.pkl', 'wb')
    pickle.dump(triangles_full_v, savefile)
    savefile.close()
    savefile = open('generated_verticies.pkl', 'wb')
    pickle.dump(affine_copy, savefile)
    savefile.close()
    savefile = open('small.pkl', 'wb')
    pickle.dump(smallpolygons, savefile)
    savefile.close()



while 1:
    newt = clock.tick(FRAMES_PER_SECOND)
    pygame.display.flip()


    event = pygame.event.poll()
    if event.type == pygame.QUIT:
        pygame.quit()
        exit()