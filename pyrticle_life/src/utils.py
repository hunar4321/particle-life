import numpy as np

def random_xy(window_size: int, n: int =1) -> np.ndarray:
    return np.random.randint(0, window_size, 2*n).reshape(n, 2)