## A simple Python port - You need: pip install pygame. Note the code here is not efficient but it's made to be educational and easy
import pygame
import numpy as np
import random

from src.bodies import *
from src import rules

bodies=[]
window_size = 800
pygame.init()
window = pygame.display.set_mode((window_size, window_size))


def draw(surface, x, y, color, size: float):
    for i in range(0, size):
        pygame.draw.line(surface, color, (x, y-1), (x, y+2), abs(size))


# Create the atoms color teams
yellow = create_group(
    Atom, 100, color="yellow", bodies=bodies, window_size=window_size)
red = create_group(
    Atom, 100, color="red", bodies=bodies, window_size=window_size)
blue = create_group(
    MassBody, 10, color="blue", bodies=bodies, window_size=window_size, mass=5)



run = True
while run:
    window.fill(0)
    rules.potential_energy(red, red, 0.1, window_size)
    rules.potential_energy(red, yellow, -0.15, window_size)
    rules.potential_energy(yellow, yellow, -0.1, window_size)

    rules.potential_energy(blue, red, 10, window_size)
    rules.potential_energy(blue, yellow, 10, window_size)
    rules.gravity(blue, blue, 10, window_size)
    for i in range(len(bodies)):
        draw(window,  bodies[i].x, bodies[i].y, bodies[i].color, size=3)
        
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    pygame.display.flip()
pygame.quit()
exit()
