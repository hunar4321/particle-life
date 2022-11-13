import numpy as np

from src.utils import random_xy

def test_random_xy():
    r_1 = random_xy(100)
    assert type(r_1) == np.ndarray
    assert r_1.shape == (1, 2)

    r_20 = random_xy(100, 20)
    assert type(r_1) == np.ndarray
    assert r_20.shape == (20, 2)


