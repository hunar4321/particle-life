""" Bodies
Here the bodies definitions are collected.

"""

import numpy as np

from src.utils import random_xy

def atom(x, y, c):
    "Define an atom by position (x,y) and color"
    return {"x": x, "y": y, "vx": 0, "vy": 0, "color": c}

def create_atoms(number, color, bodies, window_size):
    """Create a new bodies group and add them to `bodies` list"""
    group = []
    for i in range(number):
        x, y = random_xy(window_size)[0]
        new_body = atom(x, y, color)
        group.append(new_body)
        bodies.append(new_body)
    return group