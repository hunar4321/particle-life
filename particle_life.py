## A simple Python port - You need: pip install pygame. Note the code here is not efficient but it's made to be educational and easy
import pygame
import random

atoms=[]
window_size = 800
pygame.init()
window = pygame.display.set_mode((window_size, window_size))


def draw(surface, x, y, color, size: float):
    for i in range(0, size):
        pygame.draw.line(surface, color, (x, y-1), (x, y+2), abs(size))
               
def atom(x, y, c):
    "Define an atom by position (x,y) and color"
    return {"x": x, "y": y, "vx": 0, "vy": 0, "color": c}

def randomxy():
    return round(random.random()*window_size + 1)

def create(number, color, atoms):
    """Create a new atoms group and add them to `atoms` list"""
    group = []
    for i in range(number):
        new_atom = atom(randomxy(), randomxy(), color)
        group.append(new_atom)
        atoms.append(new_atom)
    return group

# `up` are atoms1 update rules #

def up_potential_energy(atoms1:dict, atoms2:dict, g: int) -> None:
    """Update rule between two atoms groups

    Args:
        atoms1 (dict): The atoms group to update
        atoms2 (dict): The atoms group with which `atoms1` interacts
        g (int): 
    """
    for i in range(len(atoms1)):
        fx = 0
        fy = 0
        for j in range(len(atoms2)):
            a = atoms1[i]
            b = atoms2[j]
            dx = a["x"] - b["x"]
            dy = a["y"] - b["y"]
            d = (dx*dx + dy*dy)**0.5
            if( 0 < d < 80):
                F = g/d
                fx += F*dx
                fy += F*dy
        a["vx"] = (a["vx"] + fx)*0.5
        a["vy"] = (a["vy"] + fy)*0.5
        a["x"] += a["vx"]
        a["y"] += a["vy"]
        if(a["x"] <= 0 or a["x"] >= window_size):
            a["vx"] *=-1
        if(a["y"] <= 0 or a["y"] >= window_size):
            a["vy"] *=-1
    None   

# Create the atoms color teams
yellow = create(100, "yellow", atoms)
red = create(100, "red", atoms)
blue = create(10, "blue", atoms)



run = True
while run:
    window.fill(0)
    up_potential_energy(red, red, 0.1)
    up_potential_energy(red, yellow, -0.15)
    up_potential_energy(yellow, yellow, -0.1)

    up_potential_energy(blue, red, 10)
    up_potential_energy(blue, yellow, 10)
    for i in range(len(atoms)):
        draw(window,  atoms[i]["x"], atoms[i]["y"], atoms[i]["color"], size=3)
        
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
    pygame.display.flip()
pygame.quit()
exit()
