""" Rules

Here are defined the update rules for the bodies.
"""

def potential_energy(bodies1:dict, bodies2:dict, g: int, window_size:int) -> None:
    """Update rule between two atoms groups

    Args:
        bodies1 (dict): The atoms group to update
        bodies2 (dict): The atoms group with which `bodies1` interacts
        g (int): 
    """
    for i in range(len(bodies1)):
        fx = 0
        fy = 0
        for j in range(len(bodies2)):
            a = bodies1[i]
            b = bodies2[j]
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