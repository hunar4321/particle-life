""" Bodies
Here the bodies definitions are collected.

"""
from typing import Iterable
import numpy as np

from src.utils import random_xy


def create_group(
    species: callable,
    number: int,
    color: str,
    bodies: Iterable,
    window_size: int,
    **kwargs
):
    """Create a new bodies group and add them to `bodies` list"""
    group = []
    for i in range(number):
        new_body = species(color, window_size=window_size, **kwargs)
        group.append(new_body)
        bodies.append(new_body)
    return group

class Atom():

    def __init__(
        self,
        color: str,
        x: int = None,
        y: int = None, 
        vx: int = None,
        vy: int = None,
        window_size: int = None
    ):
        rand_x, rand_y = random_xy(n=1, window_size=window_size)[0]
        self.x = x or rand_x
        self.y = y or rand_y
        self.vx = vx or 0
        self.vy = vy or 0
        self.color = color


class MassBody(Atom):

    def __init__(
        self,
        color: str,
        x: int = None,
        y: int = None, 
        vx: int = None,
        vy: int = None,
        window_size: int = None,
        mass: float = None,
    ):
        if mass is None or mass < 0:
            raise ValueError("Mass must be a positive number")
        super().__init__(
            color=color,
            x=x,
            y=y,
            vx=vx,
            vy=vy,
            window_size=window_size,
        )
        self.mass = mass




